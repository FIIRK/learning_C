/*** includes ***/

#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

/*
<termios.h>
struct termios -> terminal attribute container
tcgetattr() -> gets attribute of terminal
tcsetattr() -> sets attribute of terminal
TCSAFLUSH (2) -> some input for tcsetattr(), i/o sequence shit

terminal attributes:
    lflag:
        ISIG    (0000001) -> process signals (for ctrl-C and ctrl-Z)
        ICANON  (0000002) -> canonical mode
        ECHO    (0000010) -> , echo input
        IEXTEN  (0100000) -> control char to literal (for, ctrl-V)
    iflag:
        BRKINT  (0000002) -> break condition sends SIGINT
        INPCK   (0000020) -> parity checking
        INSTRIP (0000040) -> strip 8th bit
        ICRNL   (0000400) -> carriage return to new line (for ctrl-M)
        IXON    (0002000) -> software flow control (for ctrl-S, ctrl-Q)
    oflag:
        OPOST   (0000001) -> output post processing (converts "\n" to "\r\n")
    cflag:
        CS8     (0000060) -> set character size to 8 bit
    cc:
        VTIME   (5) -> set maximum amount of time to wait before read() returns.
        VMIN    (6) -> set minimum number of bytes of input before read() can return

<unistd.h>
read() -> reads data
write() -> write data
STDIN_FILENO (0) -> standard input (what is that?)
STDOUT_FILENO (1) -> standard output

<ctype.h>
iscntrl() -> checks if byte read is control character

<stdlib.h>
exit() -> exits with error code
atexit() -> runs function upon exiting

<stdio.h>
printf() -> prints formatted output
perror() -> prints descriptive error message from global errno variable

<errno.h>
errno -> global variable for error code
EAGAIN (11) -> error code, read() timeout
*/

/*** defines ***/

#define CTRL_KEY(k) ((k) & 0x1f)

/*** terminal ***/

void die(const char *s) {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgattr");
    atexit(disableRawMode);

    struct termios raw = orig_termios;
    raw.c_iflag &= ~(ICRNL | IXON | BRKINT | INPCK | ISTRIP);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

char editorReadKey() {
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN) die("read");
    }
    return c;
}

/*** output ***/
void editorRefreshScreen() {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
}

/*** input ***/

void editorProcessKeypress() {
    char c = editorReadKey();

    switch (c) {
        case CTRL_KEY('q'):
            write(STDOUT_FILENO, "\x1b[2J", 4);
            write(STDOUT_FILENO, "\x1b[H", 3);

            exit(0);
            break;
    }
}

/*** init ***/

int main(){
    enableRawMode();

    while (1) {
        editorRefreshScreen();
        editorProcessKeypress();
    }
    return 0;
}