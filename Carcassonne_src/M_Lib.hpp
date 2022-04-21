// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifndef _M_Lib_hpp_
#define _M_Lib_hpp_
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifndef LINUX_APP 
	#define WINDOWS_APP 
#endif

#ifdef LINUX_APP
	#define __stdcall
	#include <dlfcn.h>	// необходимо указать линкеру внешнюю библиотеку -Wl,--no-as-needed -ldl
	#include <link.h>
	#include <string.h>
	#include <unistd.h>
	
	#define lstrcmpiA strcasecmp
#endif

#ifdef WINDOWS_APP
	#include <windows.h>
#endif 

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// класс загрузки динамических библиотек и получение адресов функций
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class M_Lib
{
public:
	M_Lib();
	~M_Lib();

	bool	__stdcall	LoadLibraryA(const char* path);
	void*	__stdcall	GetProcAddress(const char* function_name);
	void	__stdcall	FreeLibrary();

	void*			hLib;
	unsigned long	ErrorCode;
};
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Класс декодирования путей в файловых системах 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class M_LibPaths
{
public:
	M_LibPaths();
	~M_LibPaths();

	bool	__stdcall	GetModulePathByName(const char* name);
	bool	__stdcall	GetModulePathByHandle(void* lib_handle);
	bool	__stdcall	DecodePath(const char* path);
	void	__stdcall	ConvertPathToLinuxPath(char* path);

	char	file_path[4096];
	char	file_name[4096];
	char	file_name_ext[4096];
	char	file_ext[4096];
	char	file_dir[4096];

private:

	void	__stdcall	DecodePath_private();
	void	__stdcall	Clear_private();
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//
// класс загрузки динамических библиотек и получение адресов функций
//
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

inline M_Lib::M_Lib()
{
	ErrorCode = 0;
	hLib = 0;
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline M_Lib::~M_Lib()
{
	this->FreeLibrary();
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline bool	__stdcall
M_Lib::LoadLibraryA(const char* path)
{
	ErrorCode = 0;
	
	#ifdef WINDOWS_APP
		hLib = ::LoadLibraryA(path);
		if (hLib == 0)
		{
			ErrorCode = GetLastError();
			return false;
		}
	#endif

	#ifdef LINUX_APP
		hLib = dlopen(path, RTLD_LAZY | RTLD_LOCAL);
		if (hLib == 0)
		{
			printf("[-] M_Lib::LoadLibraryA -> error:\n\tpath = %s\n\tdesc = %s\n", path, dlerror());
			return false;
		}
		else
		{
			printf("[+] M_Lib::LoadLibraryA(\"%s\") = %p\n", path, hLib);
		}
	#endif

	return true;
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline void* __stdcall
M_Lib::GetProcAddress(const char* function_name)
{
	void * handle;
	
	ErrorCode = 0;

	#ifdef WINDOWS_APP
        handle = (void *)::GetProcAddress((HMODULE)hLib, function_name);
	#endif

	#ifdef LINUX_APP
		handle = dlsym(hLib, function_name);
		printf("\t[!] M_Lib::GetProcAddress(\"%s\") = %p\n", function_name, handle);
	#endif

	return handle;
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline void	__stdcall
M_Lib::FreeLibrary()
{
	#ifdef WINDOWS_APP
		if (hLib != 0) ::FreeLibrary((HMODULE)hLib);
	#endif

	#ifdef LINUX_APP
		if (hLib != 0) dlclose(hLib);
	#endif
	
	hLib = 0;
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//
// Класс декодирования путей в файловых системах 
//
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline M_LibPaths::M_LibPaths()
{
	Clear_private();
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline M_LibPaths::~M_LibPaths()
{

}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef LINUX_APP

inline int M_ModulesInfo_by_name(struct dl_phdr_info* info, size_t size, void* data)
{
	dl_phdr_info*	info_find;
	size_t			len, s;
	const char*		name;

	// смотрим что ищем
	info_find = (dl_phdr_info*)data;
	if (info_find == 0) return -1;

	// извлекаем название модуля
	name = info->dlpi_name;
	if (name == 0)
		return 0;
	len = strlen(name);
	// ищием название модуля, т.е. последний с конца слеш
	s = 0 - 1;
	while (len != s)
	{
		if ((name[len] == '/') || (name[len] == '\\'))
		{
			name = &name[len] + 1;
			break;
		}
		len = len - 1;
	}

	if (lstrcmpiA(name, info_find->dlpi_name) == 0)
	{
		info_find->dlpi_addr = info->dlpi_addr;
		info_find->dlpi_name = info->dlpi_name;
		return -1;
	}
	return 0;
}

#endif
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline bool	__stdcall
M_LibPaths::GetModulePathByName(const char* name)
{
	Clear_private();

#ifdef WINDOWS_APP
	void* lib;
	// получение указателя на image dll
	lib = GetModuleHandleA(name);
	if (lib == 0)
		return false;
	// получаем название приложения и полный путь к нему
	if (GetModuleFileNameA((HMODULE)lib, file_path, 4096) == 0)
		return false;
#endif


#ifdef LINUX_APP
	dl_phdr_info	info;
	size_t			len;
	
	// проверяем нужно ли нам искать модуль или само приложение
	if (name == 0)
	{
		if (readlink("/proc/self/exe", file_path, 4096) == -1)
		{
			return false;
		}
	}
	else
	{
		info.dlpi_addr = 0;
		info.dlpi_name = name;
		// перечисляем все модули приложения
		dl_iterate_phdr(M_ModulesInfo_by_name, &info);
		// смотрим, нашли ли?
		if ((info.dlpi_addr == 0) || (info.dlpi_name == 0))
		{
			return false;
		}
		// копируем 
		len = strlen(info.dlpi_name)+1;
		if (4096 < len)
		{
			return false;
		}
		strcpy(file_path, info.dlpi_name);
	}
#endif

	if (file_path[0] != 0)
		DecodePath_private();

	return true;
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline bool	__stdcall
M_LibPaths::GetModulePathByHandle(void* lib_handle)
{
	Clear_private();

#ifdef WINDOWS_APP
	// получаем название приложения и полный путь к нему
	if (GetModuleFileNameA((HMODULE)lib_handle, file_path, 4096) == 0)
		return false;

#endif

#ifdef LINUX_APP
	link_map*	lm;
	size_t		len;

	// проверяем нужно ли нам искать модуль или само приложение
	if (lib_handle == 0)
	{
		if (readlink("/proc/self/exe", file_path, 4096) == -1)
		{
			return false;
		}
	}
	else
	{
		if (dlinfo(lib_handle, RTLD_DI_LINKMAP, &lm) == -1)
		{
			return false;
		}

		if (lm == 0)
		{
			return false;
		}

		if (lm->l_name == 0)
		{
			return false;
		}

		// копируем 
		len = strlen(lm->l_name) + 1;
		if (4096 < len)
		{
			return false;
		}
		strcpy(file_path, lm->l_name);
	}
#endif

	if (file_path[0] != 0)
		DecodePath_private();

	return true;
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline bool	__stdcall
M_LibPaths::DecodePath(const char* path)
{
	size_t len;
	// если указатель на путь совпадает с внутренним указателем, то все затирать не надо
	if (path == &file_path[0])
	{
		file_name[0] = 0; file_ext[0] = 0; file_name_ext[0] = 0;  file_dir[0] = 0;
	}
	else
	{
		Clear_private();
	}
	if (path == 0) return false;
	
	len = strlen(path);
	if (len > 4095) return false;

	strcpy(file_path, path);
	// копируем 
	if (file_path[0] != 0) 
		DecodePath_private();

	return true;
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline void	__stdcall
M_LibPaths::ConvertPathToLinuxPath(char* path)
{
	size_t len, end;

	if (path == 0) return;

	end = 0 - 1;
	len = strlen(path);

	while (len != end)
	{
		if (path[len] == '\\') path[len] = '/';
		len = len - 1;
	}
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline void	__stdcall
M_LibPaths::Clear_private()
{
	file_path[0] = 0; file_name[0] = 0; file_ext[0] = 0; file_name_ext[0] = 0;  file_dir[0] = 0;
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline void	__stdcall
M_LibPaths::DecodePath_private()
{
	size_t	len, end, ext;
	char	cur_char;

	// декодируем путь
	len = strlen(file_path);
	ext = 0 - 1;
	end = 0 - 1;
	while (len != end)
	{
		cur_char = file_path[len];
		// расширение
		if (cur_char == '.')
		{
			// расширение - это самая последняя "."
			if (file_ext[0] == 0) 
			{
				strcpy(file_ext, &file_path[len + 1]); ext = len;
			}
		}
		// поиск названия файла и каталога
		if ((cur_char == '\\') || (cur_char == '/'))
		{
			// название файла
			if (file_name[0] == 0)
			{
				strcpy(file_name, &file_path[len + 1]);
				strcpy(file_name_ext, &file_path[len + 1]);
			}
			// название каталога
			strcpy(file_dir, file_path);
			file_dir[len+1] = 0;
			// название имени файла без расширения
			if (len != ext)
			{
				ext = ext - len - 1; 
				if (ext < 4096)
					file_name_ext[ext] = 0;	
			}
			break;
		}
		len = len - 1;
	}

	// если мы не нашли название файла, то путь и есть имя файла

	if (file_name[0] == 0)
	{
		strcpy(file_name, file_path);
		strcpy(file_name_ext, file_path);
	}
		
	if (file_ext[0] != 0)
	{
		file_name_ext[ext] = 0;
	}
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif //_M_Lib_hpp_
