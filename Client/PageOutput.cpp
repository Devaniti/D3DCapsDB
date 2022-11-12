#include "PrecompiledHeader.h"
#include "PageOutput.h"
#include "ResourceManager.h"

bool GenerateAndOpenOutputPage(const std::vector<JSONStorage>& elements)
{
    std::filesystem::path folderPath = std::filesystem::temp_directory_path() / L"D3DCapsDBClientReport";
    if (!std::filesystem::is_directory(folderPath))
    {
        if (!std::filesystem::create_directory(folderPath))
        {
            return false;
        }
    }

    std::filesystem::path htmlFilePath = folderPath / L"Main.html";
    std::ofstream HTMLFile;
    HTMLFile.open(htmlFilePath, std::ios_base::out | std::ios_base::binary);
    if (HTMLFile.fail())
    {
        return false;
    }

    auto [htmlPageCode, htmlPageSize ] = GetHTMLPageCode();
    HTMLFile.write(static_cast<const char*>(htmlPageCode), htmlPageSize);
    HTMLFile.close();
    if (HTMLFile.fail())
    {
        return false;
    }

    std::filesystem::path reportsFilePath = folderPath / L"Reports.js";
    std::wofstream ReportsFile;
    ReportsFile.open(reportsFilePath, std::ios_base::out | std::ios_base::binary);
    if (ReportsFile.fail())
    {
        return false;
    }

    std::wstring jsPrefix = L"const reports = ";
    ReportsFile.write(jsPrefix.c_str(), jsPrefix.size());

    std::wstring reportData = JSONStorage::Output(elements);
    ReportsFile.write(reportData.c_str(), reportData.size());

    ReportsFile.close();
    if (ReportsFile.fail())
    {
        return false;
    }

    ::ShellExecuteW(NULL, L"open", htmlFilePath.c_str(), NULL, NULL, SW_SHOW);

    return true;
}
