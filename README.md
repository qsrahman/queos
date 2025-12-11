# Roll your own Operating System

QUEOS is a simple 32-bit x86 kernel. This is the first step in creating an
operating system.

I'm just following:

- [OSDev.org](https://wiki.osdev.org/)
- [Bran's Kernel Developments Toturial](http://www.osdever.net/bkerndev/Docs/gettingstarted.htm)
- [JamesM's kernel Development Tutorial](http://www.jamesmolloy.co.uk/tutorial_html/)
- [6.828: Operating System Engineering](https://pdos.csail.mit.edu/6.828/2011/index.html)
- [CS-537: Introduction to Operating Systems](https://pages.cs.wisc.edu/~remzi/Classes/537/Spring2018/)

Familiarity with your development environment is crucial for productive
development and debugging. Most modern Linuxes and BSDs have an ELF toolchain
already installed. That is, the system-standard gcc, as, ld and objdump should
just work. If you are using something other than standard x86 Linux or BSD, you
will need the GNU C compiler toolchain, configured and built as a cross-compiler
for the target `i686-elf`, as well as the GNU debugger, configured for the
i686-elf toolchain.
