# ArgParser-C – Usage Guide

A lightweight and structured argument parsing library for C

This document explains how to use the public API of ArgParser-C, including
function descriptions and minimal usage examples

---

# Basic Usage

## 1. Initialize the Parser

### `APC_ArgParser apc_init(int argc, char *argv[]);`

Initializes a new argument parser instance using the `argc` and `argv`
values provided to `main()`

### Example

```c
int main(int argc, char *argv[])
{
    APC_ArgParser parser = apc_init(argc, argv);

    /* More code here... */
}
```

This prepares the parser and stores argument data internally

---

## 2. Define Arguments

Arguments are described using `APC_ArgInfo`.

### `APC_ArgInfo apc_initInfo(void);`

Initializes an argument descriptor with safe defaults:

* `required` defaults to `false`
* `aliases` is automatically constructed
* All pointers are initialized to `NULL`

### Example

```c
APC_ArgInfo info = apc_initInfo();
info.id     = "version";
info.param  = "--version";
info.sparam = "-v";
info.help   = "Version information";

apc_add(&parser, info);
```

---

## 3. Register Arguments

### `bool apc_add(APC_ArgParser *argpar, APC_ArgInfo info);`

Registers a configured `APC_ArgInfo` structure with the parser

Returns:

* `true` on success
* `false` on failure

### Example with Aliases

```c
APC_ArgInfo info = apc_initInfo();
info.id     = "help";
info.param  = "--help";
info.sparam = "-h";
info.help   = "Show this help";

cvec_push(&info.aliases, const char *, "-?");

apc_add(&parser, info);
```

---

## 4. Query Arguments

### `bool apc_get(APC_ArgParser *argpar, const char *id);`

Checks whether a registered argument is present in `argv`.

Returns:

* `true` if the argument exists
* `false` otherwise

### Example

```c
if (apc_get(&parser, "version"))
{
    printf("Version: %i.%i.%i\n",
        APC_VERSION_MAJOR,
        APC_VERSION_MINOR,
        APC_VERSION_PATCH);
}
```

---

## 5. Generate Help Text

### `char *apc_generateHelp(APC_ArgParser *argpar, const char \*title, const char \*topInfo, const char \*lowerInfo);

Generates a formatted help message based on registered arguments

Parameters:

* `title` — Help title
* `topInfo` — Optional description or introduction
* `lowerInfo` — Optional footer text (license, notes, etc...)

Returns:

* Dynamically allocated string (must be freed)

### Example

```c
else if (apc_get(&parser, "help"))
{
    const char *license =
        "\nVery cool license\n";

    char *helpMsg = apc_generateHelp(
        &parser,
        "APC Help",
        "ArgParser-C demonstration program\n",
        license);

    printf("%s\n", helpMsg);

    APC_FREE(helpMsg); // Internal automatic free() function
}
```

---

## 6. Cleanup

### `void apc_destroy(APC_ArgParser *argpar);`

Frees all allocated resources used by the parser

### Example

```c
apc_destroy(&parser);
return 0;
}
```

---

# Public API Reference

Below is a summary of the non-private (public) API functions

---

## `apc_init`

Initializes the argument parser

```c
APC_ArgParser apc_init(int argc, char *argv[]);
```

---

## `apc_initInfo`

Creates a clean `APC_ArgInfo` structure

```c
APC_ArgInfo apc_initInfo(void);
```

---

## `apc_add`

Registers an argument definition

```c
bool apc_add(APC_ArgParser *argpar, APC_ArgInfo info);
```

---

## `apc_get`

Checks if a registered argument exists

```c
bool apc_get(APC_ArgParser *argpar, const char *id);
```

---

## `apc_generateHelp`

Automatically builds formatted help text

```c
char *apc_generateHelp(APC_ArgParser *argpar,
                       const char *title,
                       const char *topInfo,
                       const char *lowerInfo);
```

Memory ownership:
The returned string must be released using `APC_FREE()`.

---

## `apc_destroy`

Destroys the parser and frees all internal allocations

```c
void apc_destroy(APC_ArgParser *argpar);
```

---

# Summary

Typical usage:

1. Initialize parser (`apc_init`)
2. Create argument definitions that can be modified (`apc_initInfo`)
3. Register arguments (`apc_add`)
4. Query arguments (`apc_get`)
5. Optionally generate help (`apc_generateHelp`)
6. Destroy parser (`apc_destroy`)

The API is intentionally minimal and explicit, designed to remain simple,
predictable, and suitable for low-level C projects

