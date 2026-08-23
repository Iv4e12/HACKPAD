#include "ciphers.h"
#include <cstring>

const char THERAPRISM_ALPHABET[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXY";

const char FORDESE_ALPHABET[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

const char BILL_ALPHABET[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ";


String processText(const String &text, const char *alphabet)
{
    String result = "";

    for (unsigned int i = 0; i < text.length(); i++)
    {
        char character = text.charAt(i);

        if (character >= 'a' && character <= 'z')
        {
            character -= 32;
        }

        if (strchr(alphabet, character) != nullptr)
        {
            result += character;
        }

        else if (character == ' ')
        {
            result += ' ';
        }

        
        else
        {
            result += '?';
        }
    }

    return result;
}



String theraprismEncode(const String &text)
{
    return processText(text, THERAPRISM_ALPHABET);
}

String theraprismDecode(const String &text)
{
    return processText(text, THERAPRISM_ALPHABET);
}



String fordeseEncode(const String &text)
{
    return processText(text, FORDESE_ALPHABET);
}

String fordeseDecode(const String &text)
{
    return processText(text, FORDESE_ALPHABET);
}

String billEncode(const String &text)
{
    return processText(text, BILL_ALPHABET);
}

String billDecode(const String &text)
{
    return processText(text, BILL_ALPHABET);
}