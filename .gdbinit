set $lastcs = -1

define hook-stop
  # There doesn't seem to be a good way to detect if we're in 16- or
  # 32-bit mode, but in 32-bit mode we always run with CS == 8 in the
  # kernel and CS == 35 in user space
  if $cs == 8 || $cs == 35
    if $lastcs != 8 && $lastcs != 35
      set architecture i386
    end
    x/i $pc
  else
    if $lastcs == -1 || $lastcs == 8 || $lastcs == 35
      set architecture i8086
    end
    # Translate the segment:offset into a physical address
    printf "[%4x:%4x] ", $cs, $eip
    x/i $cs*16+$eip
  end
  set $lastcs = $cs
end

set pagination off
set history expansion on
set disassembly-flavor intel

echo Connecting to QEMU\n

echo (gdb) target remote localhost:5000\n
target remote localhost:25501

echo (gdb) add-symbol-file kernel.elf\n
add-symbol-file build/kernel.elf

echo \n
echo Execution is currently paused, type "continue" to start executing.\n
echo Ctrl-C pauses execution again. \n
echo "break" sets breakpoints. \n
echo "step" and "stepi" advance by one line or instruction. \n
echo "next" is like "step" but treats subroutines as single instructions. \n
echo "print" evaluates and prints c expressions. \n
echo Type "help" followed by command name for full documentation, \n
echo or "help" for all commands.\n
echo Don't know the exact name of the command you're looking for? \n
echo Use the "apropos" command to search the help system for specific terms. \n
