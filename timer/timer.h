#ifndef timer_h
# define timer_h

/**
 * timer_sig_handler: a function which is needed to receive an alarm and send a custom signal
 * to the manager.
 */
void timer_sig_handler(int signum);

#endif