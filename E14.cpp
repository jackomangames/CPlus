#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <chrono>
#include <thread>

#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include <sys/ioctl.h>

using namespace std::chrono;

const milliseconds FPS(80);

bool stop = false;

namespace term {
	const unsigned MAX_COUNT_COLOR = 256;

	size_t cols() {
		winsize size;
		ioctl(0, TIOCGWINSZ, &size);
		return size.ws_col;
	}

	size_t rows() {
		winsize size;
		ioctl(0, TIOCGWINSZ, &size);
		return size.ws_row;
	}

	void gotoxy(int x, int y) {
		std::cout << "\e[" << y << ';' << x << 'H' << std::flush;
	}

	std::ostream& cls(std::ostream& out) {
		out << "\033[2J\033[1;1H";
		return out;
	}

	std::ostream& resetcolor(std::ostream& out) {
		out << "\e[0m";
		return out;
	}

	class omanip {
	private:
		std::string buf;
	public:
		omanip(const std::string& buf);
		friend std::ostream& operator<<(std::ostream& out, const omanip& manip);
	};

	omanip::omanip(const std::string& buf)
		: buf(buf) {
	}

	std::ostream& operator<<(std::ostream& out, const omanip& manip) {
		out << manip.buf;
		return out;
	}

	omanip setcolor(unsigned code) {
		std::ostringstream out;
		out << "\e[48;5;" << code % MAX_COUNT_COLOR << "m" << std::flush;
		return omanip(out.str());
	}

	void print_color(unsigned color, unsigned n = 1) {
		std::cout << setcolor(color);
		for (unsigned i(0); i < n; i++) {
			std::cout << ' ';
		}
		std::cout << resetcolor << std::flush;
	}
}

milliseconds delta_time() {
	static auto curr = system_clock::now();
	static auto last = system_clock::now();
	static milliseconds delta(0);
	curr = system_clock::now();
	delta = duration_cast<milliseconds>(curr - last);
	last = curr;
	return delta;
}


void check_ctr_c(int signal) {
	stop = true;
}

void check_input() {
	static char buf[BUFSIZ];
	int n(0), flags(fcntl(0, F_GETFL));
	fcntl(0, F_SETFL, flags | O_NONBLOCK);
	n = read(0, buf, BUFSIZ);
	fcntl(0, F_SETFL, flags);
	if (n > 0) {
		stop = true;
	}
}

int main() {
	const size_t width(term::cols()), height(term::rows());
	const size_t width_block(3), height_block(2);
	unsigned color(term::MAX_COUNT_COLOR - 1);
	int x1(width / 2 - width_block + 1), y1(0), x2(width / 2 - width_block + 2), y2(0);

	termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	signal(SIGINT, check_ctr_c);
	std::cout << term::cls;
	
	while (!stop) {
		check_input();
		term::gotoxy(x1, y1);
		for (int i(0); i < height_block && y1 <= height; i++) {
			//term::gotoxy(x1, y1++);
			term::print_color(color, !x1 ? width % width_block : width_block);
			term::gotoxy(x1, ++y1);
		}
		
		term::gotoxy(x2, y2);
		for (int i(0); i < height_block && y2 <= height; i++) {
			//term::gotoxy(x2, y2++);
			term::print_color(color, !x2 ? width % width_block : width_block);
			term::gotoxy(x2, ++y2);
		}

		if (y1 == height + 1) {
			if (!x1) {
				std::cout << term::cls;
				x1 = width / 2 - width_block + 1;
				x2 = width / 2 - width_block + 2;
				y1 = 0;
				y2 = 0;
				//term::gotoxy(x1, y1);
			} else if (x1 < width_block) {
				x1 = 0;
			} else {
				x1 -= width_block;
				x2 += width_block;
			}
			y1 = 0;
			y2 = 0;
		}

		color = !color ? term::MAX_COUNT_COLOR - 1 : color - 1;

		std::this_thread::sleep_for(FPS - delta_time());
	}

	std::cout << term::cls;
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return 0;
}

