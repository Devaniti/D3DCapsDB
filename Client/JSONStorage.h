#pragma once

#include <string>
#include <vector>
#include <stack>

class JSONStorage
{
public:
    JSONStorage() = default;
    ~JSONStorage();

    JSONStorage(const JSONStorage& other) = delete;
    JSONStorage(JSONStorage&& other);

    JSONStorage& operator=(const JSONStorage& other) = delete;
    JSONStorage& operator=(JSONStorage&& other) = delete;

    void PushStruct(const wchar_t* structName);
    void PopStruct();

    void AddEntry(const wchar_t* name, int64_t data);
    void AddEntry(const wchar_t* name, const wchar_t* data);

    static std::wstring Output(const std::vector<JSONStorage>& elements);
    std::wstring Output();

private:
    void Output(std::wostringstream& stringBuilder) const;

    struct Entry
    {
        Entry() = default;
        ~Entry() = default;

        enum DataType
        {
            Integer,
            String
        };
        const wchar_t* name;
        DataType dataType;
        union Data
        {
            int64_t integer;
            const wchar_t* string;
        } data;
    };

    const wchar_t* CreatePropertyNameString(const wchar_t* name) const;
    const wchar_t* DuplicateString(const wchar_t* str) const;
    static void FreeString(const wchar_t* string);

    static const wchar_t* ms_Separator;

    std::vector<const wchar_t*> m_StructStack;
    std::vector<Entry> m_Data;
};
