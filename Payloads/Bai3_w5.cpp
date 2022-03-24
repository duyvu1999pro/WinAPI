#include "CWinCredentials.h"

void getCredInfo(PCREDENTIAL pCredential);
void getCredHexDump(PCREDENTIAL pCredential);
int main()
{
	CWinCredentials Credential;	
	if (Credential.Init())
	{
		PCREDENTIAL* pCredArray = NULL;
		DWORD total = 0;

		if (Credential.CredEnumerate(NULL, 0, &total, &pCredArray))
		{
			for (DWORD i = 0; i < total; i++)
			{
				PCREDENTIAL pCredential = pCredArray[i];				
				getCredInfo(pCredential);
				getCredHexDump(pCredential);
			}
			Credential.CredFree(pCredArray);
		}
	}
	else	
		printf("Failed to load the Credentials API functions !\r\n");
	return 0;
}
void getCredInfo(PCREDENTIAL pCredential)
{
	printf("\r\n\r\n");
	printf("*********************************************\r\n");
	printf("Flags:   %d\r\nType:    %d\r\nName:    %ls\r\nComment: %ls\r\nPersist: %d\r\nUser:    %ls\r\n",
		pCredential->Flags, pCredential->Type, pCredential->TargetName,
		pCredential->Comment, pCredential->Persist, pCredential->UserName);
	printf("Data: \r\n");
}
void getCredHexDump(PCREDENTIAL pCredential)
{
	char szHexBuffer[256] = "";
	char szAsciiBuffer[256] = "";
	char szHex[16];
	char szAscii[2];
	DWORD dwByte;

	//Write the credential's data as Hex Dump.
	for (dwByte = 0; dwByte < pCredential->CredentialBlobSize; dwByte++)
	{
		BYTE byte1 = pCredential->CredentialBlob[dwByte];
		sprintf_s(szHex, "%2.2X ", byte1);
		szAscii[1] = '\0';

		if (byte1 >= 32 && byte1 < 128)
			szAscii[0] = (UCHAR)byte1;
		else
			szAscii[0] = ' ';

		strcat_s(szHexBuffer, szHex);
		strcat_s(szAsciiBuffer, szAscii);

		if (dwByte == pCredential->CredentialBlobSize - 1
			|| dwByte % 16 == 15)
		{
			printf("%-50s %s\r\n", szHexBuffer, szAsciiBuffer);
			szHexBuffer[0] = '\0';
			szAsciiBuffer[0] = '\0';
		}


	}
}