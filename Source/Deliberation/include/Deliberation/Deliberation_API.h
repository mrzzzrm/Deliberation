#pragma once

#ifdef _MSC_VER 
#   define DELIBERATION_API_EXPORT_DECLARATION //__declspec(dllexport)
#   define DELIBERATION_API_IMPORT_DECLARATION //__declspec(dllimport)
#elif __GNUC__
#	define DELIBERATION_API_EXPORT_DECLARATION __attribute__ ((visibility ("default")))
#	define DELIBERATION_API_IMPORT_DECLARATION __attribute__ ((visibility ("default")))
#else
#   define DELIBERATION_API_EXPORT_DECLARATION
#   define DELIBERATION_API_IMPORT_DECLARATION
#endif 

#ifndef DELIBERATION_STATIC
#ifdef DELIBERATION_EXPORTS
#   define DELIBERATION_API DELIBERATION_API_EXPORT_DECLARATION
#else
#   define DELIBERATION_API DELIBERATION_API_IMPORT_DECLARATION
#endif
#else
#   define DELIBERATION_API
#endif

#ifdef N_DEBUG
#   define IF_DEBUG(statement)
#   define IF_NDEBUG(statement) statement
#else
#   define IF_DEBUG(statement) statement
#   define IF_NDEBUG(statement)
#endif // N_DEBUG

// http://stackoverflow.com/questions/18387640/how-to-deal-with-noexcept-in-visual-studio
#ifndef NOEXCEPT
#   ifdef _MSC_VER
#       define NOEXCEPT
#   else
#       define NOEXCEPT noexcept
#   endif
#endif

