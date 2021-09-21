syscall_wo_ret    _Exit               0
syscall_wo_ret    thread_exit         1
syscall_w_errno   read                2
syscall_w_errno   write               3
syscall_w_errno   close               4
syscall_w_errno   __lseek             5
syscall           set_thread_ptr      6
syscall           get_thread_ptr      7
syscall_w_errno   _ioctl              8
syscall_w_errno   fb_info             9
syscall_w_errno   _thread_create      10
syscall_w_errno   thread_count        11
syscall_w_errno   fork                12
syscall           getpid              13
syscall           get_sched_task_info 14
syscall           sysinfo             15
#ifdef BITS32
syscall_w_errno   __nanosleep_time64  16
#else
syscall_w_errno   nanosleep           16
#endif
syscall_w_errno   pipe                17
syscall_w_errno   dup2                18
syscall_w_errno   execve              19
syscall_w_errno   open                20
syscall_w_errno   waitpid             21
