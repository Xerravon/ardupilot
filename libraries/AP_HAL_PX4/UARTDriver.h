
#ifndef __AP_HAL_PX4_UARTDRIVER_H__
#define __AP_HAL_PX4_UARTDRIVER_H__

#include <AP_HAL_PX4.h>

class PX4::PX4UARTDriver : public AP_HAL::UARTDriver {
public:
    PX4UARTDriver(const char *devpath);
    /* PX4 implementations of UARTDriver virtual methods */
    void begin(uint32_t b);
    void begin(uint32_t b, uint16_t rxS, uint16_t txS);
    void end();
    void flush();
    bool is_initialized();
    void set_blocking_writes(bool blocking);
    bool tx_pending();

    /* PX4 implementations of BetterStream virtual methods */
    void print_P(const prog_char_t *pstr);
    void println_P(const prog_char_t *pstr);
    void printf(const char *pstr, ...);
    void _printf_P(const prog_char *pstr, ...);

    void vprintf(const char* fmt, va_list ap);
    void vprintf_P(const prog_char* fmt, va_list ap);

    /* PX4 implementations of Stream virtual methods */
    int16_t available();
    int16_t txspace();
    int16_t read();

    /* PX4 implementations of Print virtual methods */
    size_t write(uint8_t c);
    size_t write(const uint8_t *buffer, size_t size);

    volatile bool _initialised;
    volatile bool _in_timer;

    void set_device_path(const char *path) {
	    _devpath = path;
    }

    void _timer_tick(void);

private:
    const char *_devpath;
    int _fd;
    void _vdprintf(int fd, const char *fmt, va_list ap);

    bool _nonblocking_writes;

    // we use in-task ring buffers to reduce the system call cost
    // of ::read() and ::write() in the main loop
    uint8_t *_readbuf;
    uint16_t _readbuf_size;

    // _head is where the next available data is. _tail is where new
    // data is put
    volatile uint16_t _readbuf_head;
    volatile uint16_t _readbuf_tail;

    uint8_t *_writebuf;
    uint16_t _writebuf_size;
    volatile uint16_t _writebuf_head;
    volatile uint16_t _writebuf_tail;
};

#endif // __AP_HAL_PX4_UARTDRIVER_H__