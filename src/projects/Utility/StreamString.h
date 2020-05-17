#pragma once

using namespace System;
using namespace System::Text;
using namespace System::IO;

// Defines the data protocol for reading and writing strings on our stream 
public ref class StreamString
{
private:
    Stream^ ioStream;
    UnicodeEncoding^ streamEncoding;

public:
    StreamString(Stream^ ioStream)
    {
        this->ioStream = ioStream;
        streamEncoding = gcnew UnicodeEncoding();
    }

    String^ ReadString()
    {
        int len;

        len = ioStream->ReadByte() * 256;
        len += ioStream->ReadByte();
        array<Byte>^ inBuffer = gcnew array<Byte>(len);
        ioStream->Read(inBuffer, 0, len);

        return streamEncoding->GetString(inBuffer);
    }

    int WriteString(String^ outString)
    {
        array<Byte>^ outBuffer = streamEncoding->GetBytes(outString);
        int len = outBuffer->Length;
        if (len > UInt16::MaxValue)
        {
            len = (int)UInt16::MaxValue;
        }
        ioStream->WriteByte((Byte)(len / 256));
        ioStream->WriteByte((Byte)(len & 255));
        ioStream->Write(outBuffer, 0, len);
        ioStream->Flush();

        return outBuffer->Length + 2;
    }
};