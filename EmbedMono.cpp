// EmbedMono.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <mono/jit/jit.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/image.h>
#include <mono/metadata/debug-helpers.h>
#include <stdlib.h>
#include <string>
#include <iostream>

static void
NativeFoo () {
	printf("native method invoked");
}

//#define UDS "unity did so (have experimented)"

int 
main(int argc, char* argv[]) {
	MonoDomain *domain;
	const char *file;
	int retval;
	
	if (argc < 2){
		fprintf (stderr, "Please provide an assembly to load\n");
		return 1;
	}
	file = argv [1];

	//assuming that class_name is 100% matched to the file name 
	//UDS
	const int file_name_len = strlen(file);
	char* klass_name = (char*)malloc(file_name_len -3);
	strncpy(klass_name, file, file_name_len - 4);
	klass_name[file_name_len-4] = (char)0;

	//init domain
	//The parameter, named "file", which actually has nothing to do with the assembly file
	//hehe
	domain = mono_jit_init ("MyDomain");

	//load assemblies
	//whatif app.dll has some other dependencies? read metadata myself or ?
	MonoAssembly *lib_assem = mono_domain_assembly_open(domain, "lib.dll");
	MonoAssembly *app_assem = mono_domain_assembly_open(domain, file);
	MonoImage *lib_image = mono_assembly_get_image(lib_assem);
	MonoImage *app_image = mono_assembly_get_image(app_assem);
	
	//inject native call to base class
	mono_add_internal_call ("Lib::NativeFoo", NativeFoo);

	//get both base and derived class
	//assume all classes have unnamed namespace
	//UDS
	MonoClass *klass = mono_class_from_name_case(app_image, NULL, klass_name);
	MonoClass *base_klass = mono_class_from_name_case(lib_image, NULL, "lib");

	//init a instance
	MonoObject *obj = mono_object_new(domain, klass);
	mono_runtime_object_init(obj);

	//get and call OnStart()
	MonoMethod *method, *vt_method;
	method = mono_class_get_method_from_name(base_klass, "OnStart", 0);
	vt_method = mono_object_get_virtual_method(obj, method);
	mono_runtime_invoke(vt_method, obj, NULL, NULL);

	//prepare the OnInput
	method = mono_class_get_method_from_name(base_klass, "OnInput", 1);
	vt_method = mono_object_get_virtual_method(obj, method);

	//classical cin loop 
	std::string line;
	void *args[1];
	while (std::getline(std::cin, line))
	{
		args[0] = mono_string_new(domain, line.c_str());
		mono_runtime_invoke(vt_method, obj, args, NULL);
	}
	
	retval = mono_environment_exitcode_get ();
	
	mono_jit_cleanup (domain);
	return retval;
}


