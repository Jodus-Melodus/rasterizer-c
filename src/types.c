#include "types.h"

unsigned char toGray(Color color)
{
    return (0.299 * color.r + 0.587 * color.g + 0.114 * color.b);
}
