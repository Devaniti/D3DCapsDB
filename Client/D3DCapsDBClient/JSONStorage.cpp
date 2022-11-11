#include "PrecompiledHeader.h"

#include "JSONStorage.h"

const wchar_t* JSONStorage::ms_Separator = L"__";

JSONStorage::~JSONStorage()
{
    for (const Entry& entry : m_Data)
    {
        FreeString(entry.name);
        if (entry.dataType == Entry::String)
        {
            FreeString(entry.data.string);
        }
    }
}

JSONStorage::JSONStorage(JSONStorage&& other)
    : m_Data(std::move(other.m_Data))
    , m_StructStack(std::move(other.m_StructStack))
{
    other.m_Data.clear();
    other.m_StructStack.clear();
}

void JSONStorage::PushStruct(const wchar_t* structName)
{
    m_StructStack.push_back(structName);
}

void JSONStorage::PopStruct()
{
    assert(!m_StructStack.empty());
    m_StructStack.pop_back();
}

void JSONStorage::AddEntry(const wchar_t* name, int64_t data)
{
    const wchar_t* propertyName = CreatePropertyNameString(name);
    Entry entry{};
    entry.name = propertyName;
    entry.dataType = Entry::Integer;
    entry.data.integer = data;
    m_Data.push_back(entry);
}

void JSONStorage::AddEntry(const wchar_t* name, const wchar_t* data)
{
    const wchar_t* propertyName = CreatePropertyNameString(name);
    Entry entry{};
    entry.name = propertyName;
    entry.dataType = Entry::String;
    entry.data.string = DuplicateString(data);
    m_Data.push_back(entry);
}

std::wstring JSONStorage::Output() const
{
    std::wostringstream stringBuilder;

    stringBuilder << L"{";

    size_t count = m_Data.size();
    for (size_t i = 0; i < count; ++i)
    {
        const Entry& entry = m_Data[i];
        stringBuilder << L"\"" << entry.name << L"\":";
        switch (entry.dataType)
        {
        case Entry::Integer:
            stringBuilder << entry.data.integer;
            break;
        case Entry::String:
            stringBuilder << L"\"" << entry.data.string << L"\"";
            break;
        default:
            assert(0);
            break;
        }
        if (i != count - 1)
        {
            stringBuilder << L",";
        }
    }

    stringBuilder << L"}";
    return stringBuilder.str();
}

const wchar_t* JSONStorage::CreatePropertyNameString(const wchar_t* name) const
{
    size_t requiredSize = 1;
    for (const wchar_t* str : m_StructStack)
    {
        requiredSize += wcslen(str);
        requiredSize += wcslen(ms_Separator);
    }
    requiredSize += wcslen(name);

    wchar_t* result = new wchar_t[requiredSize];
    wchar_t* current = result;
    for (const wchar_t* str : m_StructStack)
    {
        size_t size = wcslen(str);
        memcpy(current, str, size * sizeof(wchar_t));
        current += size;
        size = wcslen(ms_Separator);
        memcpy(current, ms_Separator, size * sizeof(wchar_t));
        current += size;
    }
    size_t size = wcslen(name);
    memcpy(current, name, size * sizeof(wchar_t));
    current += size;
    *current = L'\0';

    return result;
}

const wchar_t* JSONStorage::DuplicateString(const wchar_t* str) const
{
    size_t requiredSize = wcslen(str) + 1;
    wchar_t* result = new wchar_t[requiredSize];
    memcpy(result, str, requiredSize * sizeof(wchar_t));

    return result;
}

void JSONStorage::FreeString(const wchar_t* string)
{
    delete[] string;
}
