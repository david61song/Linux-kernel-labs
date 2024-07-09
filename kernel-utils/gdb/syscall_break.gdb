# Function to set a conditional breakpoint on do_syscall_64
define break_on_syscall
    set $syscall_nr = $arg0
    break do_syscall_64 if nr == $syscall_nr
end

# Usage example: break on syscall number 1 (write)
# You can add more syscalls to break on by calling the function again
# For example, to also break on syscall number 2 (open):
# break_on_syscall 2


