#include "include/SPort.h"
#include "include/Core.h"
#include "include/console.h"
#include "include/kernel.h"
#include "include/types.h"


PORTTYPE port;

Port::Port(PORTTYPE port) {
  this->port = port;
}

Port::~Port() {}

bool Port::Init_Port() {
  Assembly _asm_;
  Console console;

  _asm_.out(port + 1, 0x00);
  _asm_.out(port + 3, 0x00);
  _asm_.out(port + 0, 0x03);
  _asm_.out(port + 1, 0x00);
  _asm_.out(port + 3, 0x03);      // 8 bits, no parity, one stop bit
  _asm_.out(port + 2, 0xC7);      // Enable FIFO, clear them, with 14-byte threshold
  _asm_.out(port + 4, 0x0B);      // IRQs enabled, RTS/DSR set

  _asm_.out(port+0, (uint8)((115200 / 9600) & 0xFF)); //HI
  _asm_.out(port + 0, (uint8)((115200 / 9600) >> 8)); //LOW

  //_asm_.out(port, 0xAE); // Test serial chip (send byte 0xAE and check if serial
  if (_asm_.in(port) != 0xAE) {
    return false; 
  }
  _asm_.out(port+4,0x0F);

  _asm_.out(port+1, 0x01);

  console.Write(itos(port));

  return true;
}

int Port::received() {
  Assembly _asm_;
  return _asm_.in(port + 5) & 1;
}
char Port::read_serial() {
  Assembly _asm_;
  while(received() == 0);
  return _asm_.in(port);
}

bool Port::is_transmit_empty() {
  Assembly _asm_;
  return _asm_.in(port+5) & 0x20;
}

void Port::write(char a) {
  Assembly _asm_;
  while(is_transmit_empty() == 0);
  _asm_.out(port, a);
}

void Port::writeline(string2 a) {
  Assembly _asm_;
  while(is_transmit_empty() == 0);

  for (uint16 i=0;i<stringlen(a);i++) {
    _asm_.out(port, a[i]);
  }
}