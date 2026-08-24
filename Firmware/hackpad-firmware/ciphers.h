#ifndef CIPHERS_H
#define CIPHERS_H

#include <Arduino.h>

String theraprismEncode(const String &text);
String theraprismDecode(const String &text);

String fordeseEncode(const String &text);
String fordeseDecode(const String &text);

String billEncode(const String &text);
String billDecode(const String &text);

#endif