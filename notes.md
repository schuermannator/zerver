# Notes

## Hacker News Comments:

1. Had a quick glance and while the content seems worthwhile I am a little saddened by the code.
There are numerous calls to strcpy() which is a buffer overflow waiting to
happen. I know this is not production code but it's just as important. This code
is to be used as a learning tool so I think it's important to use correct
idioms.  

Apologies for nitpicking, but code gets copied, often in a hurry and you get
bugs all over the place.  

One more comment: in the intro you mention that while the code is mostly tested
on Linux, it's likely it works on other Unixes such as Free/OpenBSD. You might
want to remind the reader that epoll() is Linux-only (bonus points if you
mention the BSDs have something similar called kqueue() :)).  

2. strcpy() is actually fine when used with literal/constant strings and the
   buffer is large enough. The compiler can even optimize it and issue warnings
   in case of overflow.
   However, using strcpy() with arbitrary strings, is a bad idea, even using the
safer variants (strncpy, strlcpy, ...) is not ideal. In the case of ZeroHTTPd,
the length of the path should be checked during request parsing and a 414 error
be returned in case it is too long. Once you know the maximum size of your path,
you should be able to size your buffers to do all your manipulation safely, and
it includes using strcpy/strcat. I wouldn't recommend it but it can be safe.


Done right, string manipulation in C can be very efficient, it is also very
tricky, because you need to be aware of your buffer sizes at all times. Ideally,
you should avoid copies.


All the string manipulation trickery may be a little too much for such a simple
project and it isn't the point, so I suggest maybe just add a few comments along
the lines as "this is unsafe". strncpy() is not the right way to do it, it is
just a band-aid so that instead of buffer overflows, you get truncated data,
which is usually a less severe bug.


Since we are nitpicking, the return values of system calls like recv() are not
properly checked. EINTR and EAGAIN have to be taken into account. It it rarely a
problem except when the server is overloaded. And considering that the point of
ZeroHTTPd is to measure performance, being robust to such conditions matter.


BTW, flooding your server is very educative. I wrote a small,
performance-oriented HTTP server myself (with epoll()) and that's at 100% CPU
with all queues full that you realize that you really need to read the man
pages. The Linux network stack is pretty stable under load but you need to do
your part.


Finally, thank you for your work, I had a feeling that epoll() was "the right
way" but I didn't test it. You did ;)


