    // Registry.cpp : implementation file   
    //   
       
    #include "stdafx.h"   
    #include "Registry.h"   
       
    /////////////////////////////////////////////////////////////////////////////   
    // CRegistry   
       
    CRegistry::CRegistry(HKEY hKey)   
    {   
        m_hKey=hKey;   
    }   
       
    CRegistry::~CRegistry()   
    {   
        Close();   
    }   
       
    /////////////////////////////////////////////////////////////////////////////   
    // CRegistry Functions   
       
    BOOL CRegistry::CreateKey(LPCTSTR lpSubKey)   
    {   
        ASSERT(m_hKey);   
        ASSERT(lpSubKey);   
       
        HKEY hKey;   
        DWORD dw;   
        long lReturn=RegCreateKeyEx(m_hKey,lpSubKey,0L,NULL,REG_OPTION_VOLATILE,KEY_ALL_ACCESS,NULL,&hKey,&dw);   
           
        if(lReturn==ERROR_SUCCESS)   
        {   
            m_hKey=hKey;   
            return TRUE;   
        }   
           
        return FALSE;   
           
    }   
       
    BOOL CRegistry::Open(LPCTSTR lpSubKey)   
    {   
        ASSERT(m_hKey);   
        ASSERT(lpSubKey);   
           
        HKEY hKey;   
        long lReturn=RegOpenKeyEx(m_hKey,lpSubKey,0L,KEY_ALL_ACCESS,&hKey);   
           
        if(lReturn==ERROR_SUCCESS)   
        {   
            m_hKey=hKey;   
            return TRUE;   
        }   
        return FALSE;   
           
    }   
       
    void CRegistry::Close()   
    {   
        if(m_hKey)   
        {   
            RegCloseKey(m_hKey);   
            m_hKey=NULL;   
        }   
           
    }   
       
    BOOL CRegistry::DeleteValue(LPCTSTR lpValueName)   
    {   
        ASSERT(m_hKey);   
        ASSERT(lpValueName);   
           
       
        long lReturn=RegDeleteValue(m_hKey,lpValueName);   
           
        if(lReturn==ERROR_SUCCESS)   
            return TRUE;   
        return FALSE;   
           
    }   
       
    BOOL CRegistry::DeleteKey(HKEY hKey, LPCTSTR lpSubKey)   
    {   
        ASSERT(hKey);   
        ASSERT(lpSubKey);   
           
        long lReturn=RegDeleteValue(hKey,lpSubKey);   
           
        if(lReturn==ERROR_SUCCESS)   
            return TRUE;   
        return FALSE;   
           
    }   
       
    BOOL CRegistry::Write(LPCTSTR lpSubKey, int nVal)   
    {   
        ASSERT(m_hKey);   
        ASSERT(lpSubKey);   
           
        DWORD dwValue;   
        dwValue=(DWORD)nVal;   
           
        long lReturn=RegSetValueEx(m_hKey,lpSubKey,0L,REG_DWORD,(const BYTE *) &dwValue,sizeof(DWORD));   
           
        if(lReturn==ERROR_SUCCESS)   
            return TRUE;   
           
        return FALSE;   
           
    }   
       
    BOOL CRegistry::Write(LPCTSTR lpSubKey, DWORD dwVal)   
    {   
        ASSERT(m_hKey);   
        ASSERT(lpSubKey);   
           
        long lReturn=RegSetValueEx(m_hKey,lpSubKey,0L,REG_DWORD,(const BYTE *) &dwVal,sizeof(DWORD));   
           
        if(lReturn==ERROR_SUCCESS)   
            return TRUE;   
           
        return FALSE;   
           
    }   
       
    BOOL CRegistry::Write(LPCTSTR lpValueName, LPCTSTR lpValue)   
    {   
        ASSERT(m_hKey);   
        ASSERT(lpValueName);   
        ASSERT(lpValue);       
       
        long lReturn=RegSetValueEx(m_hKey,lpValueName,0L,REG_SZ,(const BYTE *) lpValue,strlen(lpValue)+1);   
           
        if(lReturn==ERROR_SUCCESS)   
            return TRUE;   
           
        return FALSE;   
           
    }   
       
       
    BOOL CRegistry::Read(LPCTSTR lpValueName, int* pnVal)   
    {   
        ASSERT(m_hKey);   
        ASSERT(lpValueName);   
        ASSERT(pnVal);   
           
        DWORD dwType;   
        DWORD dwSize=sizeof(DWORD);   
        DWORD dwDest;   
        long lReturn=RegQueryValueEx(m_hKey,lpValueName,NULL,&dwType,(BYTE *)&dwDest,&dwSize);   
           
        if(lReturn==ERROR_SUCCESS)   
        {   
            *pnVal=(int)dwDest;   
            return TRUE;   
        }   
        return FALSE;   
           
    }   
       
    BOOL CRegistry::Read(LPCTSTR lpValueName, DWORD* pdwVal)   
    {   
        ASSERT(m_hKey);   
        ASSERT(lpValueName);   
        ASSERT(pdwVal);   
           
        DWORD dwType;   
        DWORD dwSize=sizeof(DWORD);   
        DWORD dwDest;   
        long lReturn=RegQueryValueEx(m_hKey,lpValueName,NULL,&dwType,(BYTE *)&dwDest,&dwSize);   
           
        if(lReturn==ERROR_SUCCESS)   
        {   
            *pdwVal=dwDest;   
            return TRUE;   
        }   
        return FALSE;   
           
    }   
       
       
       
       
    BOOL CRegistry::RestoreKey(LPCTSTR lpFileName)   
    {   
        ASSERT(m_hKey);   
        ASSERT(lpFileName);   
           
        long lReturn=RegRestoreKey(m_hKey,lpFileName,REG_WHOLE_HIVE_VOLATILE);   
           
        if(lReturn==ERROR_SUCCESS)   
            return TRUE;   
           
        return FALSE;   
    }   
       
    BOOL CRegistry::SaveKey(LPCTSTR lpFileName)   
    {   
        ASSERT(m_hKey);   
        ASSERT(lpFileName);   
           
        long lReturn=RegSaveKey(m_hKey,lpFileName,NULL);   
           
        if(lReturn==ERROR_SUCCESS)   
            return TRUE;   
           
        return FALSE;   
    }   
       
       
    BOOL CRegistry::Read(LPCTSTR lpValueName, CString* lpVal)   
    {   
        ASSERT(m_hKey);   
        ASSERT(lpValueName);   
        ASSERT(lpVal);   
           
        DWORD dwType;   
        DWORD dwSize=200;   
        char szString[2550];   
           
        long lReturn=RegQueryValueEx(m_hKey,lpValueName,NULL,&dwType,(BYTE *)szString,&dwSize);   
           
        if(lReturn==ERROR_SUCCESS)   
        {   
            *lpVal=szString;   
            return TRUE;   
        }   
        return FALSE;   
           
    }   
       
    /*=====================================  
     *  by wuhuiran 06-06-21  
     *  得到指定键下的项名称  
     *  执行操作前要打开读取的键  
    =======================================*/   
    BOOL CRegistry::EnumSubKeyName(CStringArray& arrKeyName)   
    {   
        arrKeyName.RemoveAll();   
           
        #define MAX_VALUE_NAME 260   
        CHAR     achClass[MAX_PATH] = "";  // buffer for class name    
        DWORD    cchClassName = MAX_PATH;  // length of class string    
        DWORD    cSubKeys;                 // number of subkeys    
        DWORD    cbMaxSubKey;              // longest subkey size    
        DWORD    cchMaxClass;              // longest class string    
        DWORD    cValues;              // number of values for key    
        DWORD    cchMaxValue;          // longest value name    
        DWORD    cbMaxValueData;       // longest value data    
        DWORD    cbSecurityDescriptor; // size of security descriptor    
        FILETIME ftLastWriteTime;      // last write time    
           
        DWORD j;    
        DWORD retValue;    
           
        CHAR  achValue[MAX_VALUE_NAME];    
        DWORD cchValue = MAX_VALUE_NAME;    
        CHAR  achBuff[80];    
           
        // Get the class name and the value count.    
        RegQueryInfoKey(m_hKey,        // key handle    
            achClass,                // buffer for class name    
            &cchClassName,           // length of class string    
            NULL,                    // reserved    
            &cSubKeys,               // number of subkeys    
            &cbMaxSubKey,            // longest subkey size    
            &cchMaxClass,            // longest class string    
            &cValues,                // number of values for this key    
            &cchMaxValue,            // longest value name    
            &cbMaxValueData,         // longest value data    
            &cbSecurityDescriptor,   // security descriptor    
            &ftLastWriteTime);       // last write time    
           
       
        if (cValues)    
        {   
            SetCursor(LoadCursor(NULL, IDC_WAIT));    
            for (j = 0, retValue = ERROR_SUCCESS;    
            j < cValues; j++)    
            {    
                cchValue = MAX_VALUE_NAME;    
                achValue[0] = '\0';    
                retValue = RegEnumValue(m_hKey, j, achValue,    
                    &cchValue,    
                    NULL,    
                    NULL,    // &dwType,    
                    NULL,    // &bData,    
                    NULL);   // &bcData    
                   
                if (retValue != (DWORD) ERROR_SUCCESS &&    
                    retValue != ERROR_INSUFFICIENT_BUFFER)    
                {    
                    wsprintf (achBuff,    
                        "Line:%d 0 based index = %d, retValue = %d, "    
                        "ValueLen = %d",    
                        __LINE__, j, retValue, cchValue);    
                    MessageBox (NULL, achBuff, "Debug", MB_OK);    
                }    
                   
                achBuff[0] = '\0';    
                   
                // Add each value to a list box.    
                if (!lstrlen(achValue))    
                    lstrcpy(achValue, "<NO NAME>");    
                wsprintf(achBuff, "%s", achValue);    
                   
                CString strKeyName(achBuff);   
                arrKeyName.Add(strKeyName);   
       
            }    
               
            SetCursor(LoadCursor(NULL, IDC_ARROW));    
        }    
       
       
        return arrKeyName.GetSize() > 0 ? TRUE : FALSE;   
       
    }   
       
    /*=========================================  
     *  by wuhuiran 06-06-21  
     *  得到当前打开键的所有键名称和值  
     *  操作前要打开键  
    ===========================================*/   
    BOOL CRegistry::EnumSubKeyNameAndValue(CStringArray& arrKeyName, CStringArray& arrKeyValue)   
    {   
        if(!EnumSubKeyName(arrKeyName))   
        {   
            return FALSE;   
        }   
       
        for(int i = 0; i < arrKeyName.GetSize(); i++)   
        {   
            CString strKeyValue;   
            Read(arrKeyName[i], &strKeyValue);   
               
            arrKeyValue.Add(strKeyValue);   
       
        }   
       
        if(arrKeyName.GetSize() > arrKeyValue.GetSize())   
        {   
            return FALSE;   
        }   
       
        return TRUE;   
       
    }  
