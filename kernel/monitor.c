#include <common.h>
#include <console.h>
#include <kbd.h>
#include <monitor.h>
#include <string.h>

#define CMDBUF_SIZE 80  // enough for one VGA text line

typedef struct command {
    const char* name;
    const char* desc;
    // return -1 to force monitor to exit
    int (*func)(int argc, char** argv, registers_t* tf);
} command_t;

static command_t commands[] = {
    { "help", "Display this list of commands", mon_help },
    { "cls", "Clear Screen", mon_cls },
    { "kerninfo", "Display information about the kernel", mon_kerninfo }
};

/***** Implementations of basic kernel monitor commands *****/

int mon_help(int argc, char** argv, registers_t* tf) {
    (void)argc;
    (void)argv;
    (void)tf;

    for (size_t i = 0; i < ARRAY_SIZE(commands); i++) {
        kprintf("%s - %s\n", commands[i].name, commands[i].desc);
    }
    return 0;
}

int mon_cls(int argc, char** argv, registers_t* tf) {
    (void)argc;
    (void)argv;
    (void)tf;

    cga_cls();
    return 0;
}

int mon_kerninfo(int argc, char** argv, registers_t* tf) {
    (void)argc;
    (void)argv;
    (void)tf;

    extern char _start[], _etext[], _edata[], _end[]; /*_entry[],*/

    kprintf("Special kernel symbols:\n");
    kprintf("  start                   %08x (phys)\n", _start);
    // kprintf("  entry  %08x (virt)  %08x (phys)\n", _entry, _entry);
    kprintf("  etext  %08x (virt)  %08x (phys)\n", _etext, _etext);
    kprintf("  edata  %08x (virt)  %08x (phys)\n", _edata, _edata);
    kprintf("  end    %08x (virt)  %08x (phys)\n", _end, _end);
    kprintf("Kernel executable memory footprint: %dKB\n",
            ROUNDUP(_end - _start, 1024) / 1024);
    return 0;
}

/***** Kernel monitor command interpreter *****/

#define WHITESPACE "\t\r\n "
#define MAXARGS 16

static int runcmd(char* buf, registers_t* tf) {
    int argc;
    char* argv[MAXARGS];

    // Parse the command buffer into whitespace-separated arguments
    argc = 0;
    argv[argc] = 0;
    while (1) {
        // gobble whitespace
        while (*buf && strchr(WHITESPACE, *buf))
            *buf++ = 0;
        if (*buf == 0)
            break;

        // save and scan past next arg
        if (argc == MAXARGS - 1) {
            kprintf("Too many arguments (max %d)\n", MAXARGS);
            return 0;
        }
        argv[argc++] = buf;
        while (*buf && !strchr(WHITESPACE, *buf))
            buf++;
    }
    argv[argc] = 0;

    // Lookup and invoke the command
    if (argc == 0)
        return 0;
    for (size_t i = 0; i < ARRAY_SIZE(commands); i++) {
        if (strcmp(argv[0], commands[i].name) == 0)
            return commands[i].func(argc, argv, tf);
    }
    kprintf("Unknown command '%s'\n", argv[0]);
    return 0;
}

void monitor(registers_t* tf) {
    char* buf;

    kprintf("Welcome to Que OS v%s kernel monitor!\n", VERSION);
    kprintf("Type 'help' for a list of commands.\n");

    while (1) {
        buf = readline("Q> ");
        if (buf != NULL)
            if (runcmd(buf, tf) < 0)
                break;
    }
}
