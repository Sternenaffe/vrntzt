// ch 2021-22
// c
// v1.0.0
// u_lib
// IO wrapper for a c dll

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  *
  */

#ifndef VRNTZT_IO_C_DLL_WRAPPER_H
#define VRNTZT_IO_C_DLL_WRAPPER_H

#ifdef VRNTZT_NEAT_C_DLL_EXPORT
#define VRNTZT_NEAT_C_DLL_API __declspec(dllexport)
#else
#define VRNTZT_NEAT_C_DLL_API __declspec(dllimport)
#endif // VRNTZT_NEAT_C_DLL_EXPORT

namespace u_lib::IO::c_dll
{
	// IO takes callback function
	// parameters: string
	// return: void
	// string is converted to char array for this purpose
	typedef void (*IO_c_callback)(char* t_string, size_t len);

	extern "C" VRNTZT_NEAT_C_DLL_API
	void set_all_streams(IO_c_callback t_callback_f);

	extern "C" VRNTZT_NEAT_C_DLL_API
	void set_out_stream(IO_c_callback t_callback_f);

	extern "C" VRNTZT_NEAT_C_DLL_API
	void set_debug_stream(IO_c_callback t_callback_f);

	extern "C" VRNTZT_NEAT_C_DLL_API
	void set_warning_stream(IO_c_callback t_callback_f);

	extern "C" VRNTZT_NEAT_C_DLL_API
	void set_error_stream(IO_c_callback t_callback_f);
}

#endif // !VRNTZT_IO_C_DLL_WRAPPER_H
