#include "shell.h"

ssize_t bufferCommandChain(info_t *info, char **chainBuffer, size_t *chainLength);

ssize_t getInput(info_t *info);

ssize_t readBuffer(info_t *info, char *buf, size_t *bytesRead);

int getLine(info_t *info, char **ptr, size_t *length);

void handleSIGINT(__attribute__((unused)) int signalNumber);

/**
 * bufferCommandChain - buffers chained commands
 * @info: parameter struct
 * @chainBuffer: address of command chain buffer
 * @chainLength: address of command chain length var
 *
 * Return: bytes read
 */
ssize_t bufferCommandChain(info_t *info, char **chainBuffer, size_t *chainLength)
{
    ssize_t bytesRead = 0;
    size_t pendingChainLength = 0;

    if (!*chainLength) /* if nothing left in the buffer, fill it */
    {
        free(*chainBuffer);
        *chainBuffer = NULL;
        signal(SIGINT, handleSIGINT);

#if USE_GETLINE
        bytesRead = getline(chainBuffer, &pendingChainLength, stdin);
#else
        bytesRead = getLine(info, chainBuffer, &pendingChainLength);
#endif

        if (bytesRead > 0)
        {
            if ((*chainBuffer)[bytesRead - 1] == '\n')
            {
                (*chainBuffer)[bytesRead - 1] = '\0'; /* remove trailing newline */
                bytesRead--;
            }

            info->lineCountFlag = 1;
            removeComments(*chainBuffer);
            buildHistoryList(info, *chainBuffer, info->historyCount++);

            *chainLength = bytesRead;
            info->cmdChainBuffer = chainBuffer;
        }
    }

    return bytesRead;
}

/**
 * getInput - gets a line minus the newline
 * @info: parameter struct
 *
 * Return: bytes read
 */
ssize_t getInput(info_t *info)
{
    static char *buf; /* the ';' command chain buffer */
    static size_t currentPosition, lastPosition, bufLength;
    ssize_t bytesRead = 0;
    char **bufPointer = &(info->arg), *p;

    _putchar(BUF_FLUSH);
    bytesRead = bufferCommandChain(info, &buf, &bufLength);
    
    if (bytesRead == -1) /* EOF */
        return -1;

    if (bufLength) /* we have commands left in the chain buffer */
    {
        lastPosition = currentPosition; /* init new iterator to current buf position */
        p = buf + currentPosition; /* get pointer for return */

        checkChain(info, buf, &lastPosition, currentPosition, bufLength);

        while (lastPosition < bufLength) /* iterate to semicolon or end */
        {
            if (isChain(info, buf, &lastPosition))
                break;

            lastPosition++;
        }

        currentPosition = lastPosition + 1; /* increment past nulled ';'' */
        
        if (currentPosition >= bufLength) /* reached end of buffer? */
        {
            currentPosition = bufLength = 0; /* reset position and length */
            info->cmdChainType = CMD_NORM;
        }

        *bufPointer = p; /* pass back pointer to current command position */
        return (_strlen(p)); /* return length of current command */
    }

    *bufPointer = buf; /* else not a chain, pass back buffer from getLine() */
    return bytesRead; /* return length of buffer from getLine() */
}

/**
 * readBuffer - reads a buffer
 * @info: parameter struct
 * @buf: buffer
 * @bytesRead: size
 *
 * Return: bytesRead
 */
ssize_t readBuffer(info_t *info, char *buf, size_t *bytesRead)
{
    ssize_t bytesReadCount = 0;

    if (*bytesRead)
        return 0;

    bytesReadCount = read(info->readfd, buf, READ_BUF_SIZE);
    
    if (bytesReadCount >= 0)
        *bytesRead = bytesReadCount;

    return bytesReadCount;
}

/**
 * getLine - gets the next line of input from STDIN
 * @info: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: bytesRead
 */
int getLine(info_t *info, char **ptr, size_t *length)
{
    static char buffer[READ_BUF_SIZE];
    static size_t currentPosition, bufferLength;
    size_t k;
    ssize_t bytesRead = 0, totalBytesRead = 0;
    char *p = NULL, *newPointer = NULL, *c;

    p = *ptr;

    if (p && length)
        totalBytesRead = *length;

    if (currentPosition == bufferLength)
    {
        currentPosition = bufferLength = 0;
    }

    bytesRead = readBuffer(info, buffer, &bufferLength);
    
    if (bytesRead == -1 || (bytesRead == 0 && bufferLength == 0))
        return -1;

    c = _strchr(buffer + currentPosition, '\n');
    k = c ? 1 + (unsigned int)(c - buffer) : bufferLength;
    
    newPointer = _realloc(p, totalBytesRead, totalBytesRead ? totalBytesRead + k : k + 1);

    if (!newPointer) /* MALLOC FAILURE! */
        return p ? free(p), -1 : -1;

    if (totalBytesRead)
        _strncat(newPointer, buffer + currentPosition, k - currentPosition);
    else
        _strncpy(newPointer, buffer + currentPosition, k - currentPosition + 1);

    totalBytesRead += k - currentPosition;
    currentPosition = k;
    p = newPointer;

    if (length)
        *length = totalBytesRead;

    *ptr = p;
    return totalBytesRead;
}

/**
 * handleSIGINT - blocks ctrl-C
 * @signalNumber: the signal number
 *
 * Return: void
 */
void handleSIGINT(__attribute__((unused)) int signalNumber)
{
    _puts("\n");
    _puts("$ ");
    _putchar(BUF_FLUSH);
}

