fjinit - a process reaper for firejail
======================================

fjinit is a process reaper based on [sinit][1], but with a few tweaks.
After removing some features I thought that it no longer deserved to be called
an init system.

Why?
----

Processes like Skype running in firejail aren't expecting to be PID 1, so they
don't reap any processes left behind by their parents. This fixes that problem.

If you have ever had a lot of defunct processes in firejail that can't be
killed, then this project is probably for you.

What?
-----

This is a simple daemon which responds to SIGCHLD by reaping the process.
It takes the command to run as an argument, and will fork and run it.
If it is not running as PID 1, it will just fork and exec without staying
around to reap processes.

If it started as PID 1, this will stay around until interrupted or killed.

[1]: http://tools.suckless.org/sinit "sinit homepage"
