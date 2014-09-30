 /*
 *  Copyright © 2013 Federico Vaga <federico.vaga@gmail.com>
 *  License: GPLv2
 */

#include <stdio.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include <libuinput.h>

static struct uinput_user_dev uudev = {
		.name = "libuinput-keyboard-demo",
		.id = {
				.bustype = BUS_USB,
				.vendor = 0x1,
				.product = 0x1,
				.version = 1,
		},
};
static struct input_event events[] = {
		{{0, 0}, EV_KEY, KEY_Q, 1},
		{{0, 0}, EV_KEY, KEY_Q, 0},
		{{0, 0}, EV_KEY, KEY_W, 1},
		{{0, 0}, EV_KEY, KEY_W, 0},
		{{0, 0}, EV_KEY, KEY_E, 1},
		{{0, 0}, EV_KEY, KEY_E, 0},
		{{0, 0}, EV_KEY, KEY_R, 1},
		{{0, 0}, EV_KEY, KEY_R, 0},
		{{0, 0}, EV_KEY, KEY_T, 1},
		{{0, 0}, EV_KEY, KEY_T, 0},
		{{0, 0}, EV_KEY, KEY_Y, 1},
		{{0, 0}, EV_KEY, KEY_Y, 0},
};

static struct input_event qevents[] = {
                {{0, 0}, EV_KEY, KEY_Q, 1},
                {{0, 0}, EV_KEY, KEY_Q, 0},
};

static struct input_event wevents[] = {
                {{0, 0}, EV_KEY, KEY_W, 1},
                {{0, 0}, EV_KEY, KEY_W, 0},
};


int mygetch ( void )
{
  int ch;
  struct termios oldt, newt;

  tcgetattr ( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );

  return ch;
}


int main(int argc, char *argv[]) {
	struct uinput_tkn *tkn;
	int err;

	tkn = uinput_open(NULL);
	if (!tkn) {
		fprintf(stderr, "Cannot open uinput device\n");
		exit(1);
	}

	/* Enable keyboard events */
	err = uinput_enable_event(tkn, EV_KEY);
	/* Enable a list of keyboard events */
	err = uinput_set_valid_event(tkn, UI_SET_KEYBIT, KEY_Q);
	if (err)
	  fprintf(stderr, "Cannot set event KEY_Q");
	err = uinput_set_valid_event(tkn, UI_SET_KEYBIT, KEY_W);
	if (err)
	  fprintf(stderr, "Cannot set event KEY_W");
	err = uinput_set_valid_event(tkn, UI_SET_KEYBIT, KEY_E);
	if (err)
	  fprintf(stderr, "Cannot set event KEY_E");
	err = uinput_set_valid_event(tkn, UI_SET_KEYBIT, KEY_R);
	if (err)
	  fprintf(stderr, "Cannot set event KEY_R");
	err = uinput_set_valid_event(tkn, UI_SET_KEYBIT, KEY_T);
	if (err)
	  fprintf(stderr, "Cannot set event KEY_T");
        err = uinput_set_valid_event(tkn, UI_SET_KEYBIT, KEY_Y);
        if (err)
          fprintf(stderr, "Cannot set event KEY_Y");

	/* Create uinput device */
	err = uinput_create_new_device(tkn, &uudev);
	printf("input%d virtual device created. Read events from %s\n\n",
		uinput_get_input_no(tkn), uinput_get_event_path(tkn));

	printf("To dump events on this device, run the command:"
	       "\nxinput test \"libuinput-keyboard-demo\"\n\n");

	printf("Send some events in 10 seconds. We are trying to write \"qwerty\", but "
	       "you should see the word \"qwery\" on your command line because "
	       "\"t\" event was not set\n");
	sleep(10);
        printf("start ./evtest and press any key to continue and send some events from here\n");
        mygetch();
	/* Simulate some events */
	err = uinput_send_events(tkn, events, 12, 1);
	if (err) {
		fprintf(stderr, "Cannot inject event uinput device\n");
	}
        printf("press any key for an attempt to send one event\n");
        mygetch();
        //err = uinput_send_events(tkn, events, 1, 1);
        err = uinput_send_events(tkn, qevents, 2, 1);
        err = uinput_send_events(tkn, wevents, 2, 1);
        if (err) {
                fprintf(stderr, "Cannot inject event uinput device\n");
        }
        printf("press any key to close the uinput demo\n");
        mygetch();
	uinput_close(tkn);
	return 0;
}
