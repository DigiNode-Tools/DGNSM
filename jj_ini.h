#ifndef __INI_H__
#define __INI_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>

    typedef int (*ini_handler)(void *user, const char *section,
                               const char *name, const char *value);

    typedef char *(*ini_reader)(char *str, int num, void *stream);

    int ini_parse(const char *filename, ini_handler handler, void *user);

    int ini_parse_file(FILE *file, ini_handler handler, void *user);

    int ini_parse_stream(ini_reader reader, void *stream, ini_handler handler,
                         void *user);

#ifndef INI_ALLOW_MULTILINE
#define INI_ALLOW_MULTILINE 1
#endif

#ifndef INI_ALLOW_BOM
#define INI_ALLOW_BOM 1
#endif

#ifndef INI_ALLOW_INLINE_COMMENTS
#define INI_ALLOW_INLINE_COMMENTS 1
#endif
#ifndef INI_INLINE_COMMENT_PREFIXES
#define INI_INLINE_COMMENT_PREFIXES ";"
#endif

#ifndef INI_USE_STACK
#define INI_USE_STACK 1
#endif

#ifndef INI_STOP_ON_FIRST_ERROR
#define INI_STOP_ON_FIRST_ERROR 0
#endif

#ifndef INI_MAX_LINE
#define INI_MAX_LINE 200
#endif

#ifdef __cplusplus
}
#endif

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#if !INI_USE_STACK
#include <stdlib.h>
#endif

#define MAX_SECTION 50
#define MAX_NAME 50

inline static char *rstrip(char *s)
{
    char *p = s + strlen(s);
    while (p > s && isspace((unsigned char)(*--p)))
        *p = '\0';
    return s;
}

inline static char *lskip(const char *s)
{
    while (*s && isspace((unsigned char)(*s)))
        s++;
    return (char *)s;
}

inline static char *find_chars_or_comment(const char *s, const char *chars)
{
#if INI_ALLOW_INLINE_COMMENTS
    int was_space = 0;
    while (*s && (!chars || !strchr(chars, *s)) &&
           !(was_space && strchr(INI_INLINE_COMMENT_PREFIXES, *s)))
    {
        was_space = isspace((unsigned char)(*s));
        s++;
    }
#else
    while (*s && (!chars || !strchr(chars, *s)))
    {
        s++;
    }
#endif
    return (char *)s;
}

inline static char *strncpy0(char *dest, const char *src, size_t size)
{
    if (size > 0)
    {
        sprintf(dest, "%.*s", (int)(size - 1), src);
    }
    dest[size - 1] = '\0';
    return dest;
}

inline int ini_parse_stream(ini_reader reader, void *stream, ini_handler handler,
                            void *user)
{

#if INI_USE_STACK
    char line[INI_MAX_LINE];
#else
    char *line;
#endif
    char section[MAX_SECTION] = "";
    char prev_name[MAX_NAME] = "";

    char *start;
    char *end;
    char *name;
    char *value;
    int lineno = 0;
    int error = 0;

#if !INI_USE_STACK
    line = (char *)malloc(INI_MAX_LINE);
    if (!line)
    {
        return -2;
    }
#endif

    while (reader(line, INI_MAX_LINE, stream) != NULL)
    {
        lineno++;

        start = line;
#if INI_ALLOW_BOM
        if (lineno == 1 && (unsigned char)start[0] == 0xEF &&
            (unsigned char)start[1] == 0xBB &&
            (unsigned char)start[2] == 0xBF)
        {
            start += 3;
        }
#endif
        start = lskip(rstrip(start));

        if (*start == ';' || *start == '#')
        {
        }
#if INI_ALLOW_MULTILINE
        else if (*prev_name && *start && start > line)
        {

#if INI_ALLOW_INLINE_COMMENTS
            end = find_chars_or_comment(start, NULL);
            if (*end)
                *end = '\0';
            rstrip(start);
#endif

            if (!handler(user, section, prev_name, start) && !error)
                error = lineno;
        }
#endif
        else if (*start == '[')
        {
            end = find_chars_or_comment(start + 1, "]");
            if (*end == ']')
            {
                *end = '\0';
                strncpy0(section, start + 1, sizeof(section));
                *prev_name = '\0';
            }
            else if (!error)
            {
                error = lineno;
            }
        }
        else if (*start)
        {
            end = find_chars_or_comment(start, "=:");
            if (*end == '=' || *end == ':')
            {
                *end = '\0';
                name = rstrip(start);
                value = lskip(end + 1);
#if INI_ALLOW_INLINE_COMMENTS
                end = find_chars_or_comment(value, NULL);
                if (*end)
                    *end = '\0';
#endif
                rstrip(value);

                strncpy0(prev_name, name, sizeof(prev_name));
                if (!handler(user, section, name, value) && !error)
                    error = lineno;
            }
            else if (!error)
            {
                error = lineno;
            }
        }

#if INI_STOP_ON_FIRST_ERROR
        if (error)
            break;
#endif
    }

#if !INI_USE_STACK
    free(line);
#endif

    return error;
}

inline int ini_parse_file(FILE *file, ini_handler handler, void *user)
{
    return ini_parse_stream((ini_reader)fgets, file, handler, user);
}

inline int ini_parse(const char *filename, ini_handler handler, void *user)
{
    FILE *file;
    int error;

    file = fopen(filename, "r");
    if (!file)
        return -1;
    error = ini_parse_file(file, handler, user);
    fclose(file);
    return error;
}

#endif

#ifndef __JJINI_H__
#define __JJINI_H__

#include <map>
#include <set>
#include <string>

class JJINI
{
public:
    JJINI(){};
    explicit JJINI(const std::string &filename);
    explicit JJINI(FILE *file);
    int ParseError() const;
    const std::set<std::string> &Sections() const;
    std::string Get(const std::string &section, const std::string &name,
                    const std::string &default_value) const;

    long GetInteger(const std::string &section, const std::string &name, long default_value) const;
    double GetReal(const std::string &section, const std::string &name, double default_value) const;
    float GetFloat(const std::string &section, const std::string &name, float default_value) const;
    bool GetBoolean(const std::string &section, const std::string &name, bool default_value) const;

protected:
    int _error;
    std::map<std::string, std::string> _values;
    std::set<std::string> _sections;
    static std::string MakeKey(const std::string &section, const std::string &name);
    static int ValueHandler(void *user, const char *section, const char *name,
                            const char *value);
};

#endif

#ifndef __JJINI__
#define __JJINI__

#include <algorithm>
#include <cctype>
#include <cstdlib>

inline JJINI::JJINI(const std::string &filename)
{
    _error = ini_parse(filename.c_str(), ValueHandler, this);
}

inline JJINI::JJINI(FILE *file)
{
    _error = ini_parse_file(file, ValueHandler, this);
}

inline int JJINI::ParseError() const
{
    return _error;
}

inline const std::set<std::string> &JJINI::Sections() const
{
    return _sections;
}

inline std::string JJINI::Get(const std::string &section, const std::string &name, const std::string &default_value) const
{
    std::string key = MakeKey(section, name);
    return _values.count(key) ? _values.at(key) : default_value;
}

inline long JJINI::GetInteger(const std::string &section, const std::string &name, long default_value) const
{
    std::string valstr = Get(section, name, "");
    const char *value = valstr.c_str();
    char *end;
    long n = strtol(value, &end, 0);
    return end > value ? n : default_value;
}

inline double JJINI::GetReal(const std::string &section, const std::string &name, double default_value) const
{
    std::string valstr = Get(section, name, "");
    const char *value = valstr.c_str();
    char *end;
    double n = strtod(value, &end);
    return end > value ? n : default_value;
}

inline float JJINI::GetFloat(const std::string &section, const std::string &name, float default_value) const
{
    std::string valstr = Get(section, name, "");
    const char *value = valstr.c_str();
    char *end;
    float n = strtof(value, &end);
    return end > value ? n : default_value;
}

inline bool JJINI::GetBoolean(const std::string &section, const std::string &name, bool default_value) const
{
    std::string valstr = Get(section, name, "");
    std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
    if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
        return true;
    else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
        return false;
    else
        return default_value;
}

inline std::string JJINI::MakeKey(const std::string &section, const std::string &name)
{
    std::string key = section + "=" + name;
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    return key;
}

inline int JJINI::ValueHandler(void *user, const char *section, const char *name,
                                   const char *value)
{
    JJINI *reader = (JJINI *)user;
    std::string key = MakeKey(section, name);
    if (reader->_values[key].size() > 0)
        reader->_values[key] += "\n";
    reader->_values[key] += value;
    reader->_sections.insert(section);
    return 1;
}

#endif
