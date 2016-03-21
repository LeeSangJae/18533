/*
 * 이상재 : 2016-03-16
 * 파일명을 입력 받으면 모든 디스크를 뒤져서 그 파일이 있는지 찾는 프로그램 
 * 있으면 파일의 경로를 보여줌
 
 < 작업 순서>
 1. 디렉터리 탐색
 2. 파일 로그
 3. 멀티스레드 (스레드 개수 세팅 가능해야 함)
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO; //파일과 디렉터리 조작 클래스
using System.Threading; //스레드
using System.Runtime.InteropServices;

[ComVisible(true)]
[Guid("A742A006-ECE7-4BC1-AD7A-A09D61A82DD7")]
public interface Interface_Charp
{
    void DirSearch(string diskDir);
    void initProcess();
    void CreateLogFile(string fileName);
    void ThreadPoolCallback(Object threadContext);
}
 
/*
public class CShapSample : ICShapSample
{
    public void ShowMessage()
    {
        Console.WriteLine("Hello World!!");
    }
}
*/


/* 프로그램에서 사용되는 고정된 상수 값을 가지고 있는 클래스 #define 과 같음 */
public class ValueClass
{
    public const string disk_path1 = @"C:\"; //경로
    public const string disk_path2 = @"D:\"; // @ : 경로를 위한 역슬래쉬 라는 것을 나타냄

    public static string filename; //입력 받을 파일명
    public static int ThreadCount = 0; //여기서 스레드 개수를 할당할 수 있다. 64개 맥시멈
    public static Mutex mutex = new Mutex(); //뮤텍스 생성
}


[ComVisible(true)]
[ClassInterface(ClassInterfaceType.None)]
[Guid("AA00F4D5-7AD4-4302-AAB5-B1F7B728C5DE")]
public class Program :Interface_Charp
{
    public Program() //기본 생성자
    {

    }
    
    /* 생성자 생성 */
    public Program(ManualResetEvent doneEvent)
    {
        _doneEvent = doneEvent;
    }
    

    // Wrapper method for use with thread pool.
    //static public void ThreadPoolCallback(Object threadContext, Object threadContext)
    public void ThreadPoolCallback(Object threadContext)
    {
        string threaddirs = (string)threadContext;
        // Console.WriteLine("[ thread {0} started... ]", threadIndex);
        Console.WriteLine("[ thread {0} started... ]", ThreadPool_set.currentThreadcount);
        p.DirSearch(threaddirs); //디렉터리를 탐색하며 호출할 수 있도록 변수 할당
        //Console.WriteLine("[ thread {0} directory search finished... ]", ThreadPool_set.currentThreadcount-1);
        ThreadPool_set.currentCountDiscount();
        _doneEvent.Set(); //스레드가 이벤트를 완료하고 다른 스레드를 깨운다.

        //Console.WriteLine("[ thread {0} directory search finished... ]", threadIndex);
    }


    /* 디렉터리 탐색 */
    public void DirSearch(string diskDir) //최초 루트 디렉터리를 입력 받아 시작함
    {
        string excpt_temp_path = diskDir; //예외가 났을 때 다시 해보기 위해
        try
        {
            string[] fileEntry_main = Directory.GetDirectories(diskDir); // 인자로 받은 디렉토리에서 파일의 개수를 구함
            int fileEntry_len_main = fileEntry_main.Length; // 길이를 구함

            //foreach (string subdirectory in Directory.GetDirectories(diskDir)) // 디렉터리만 보는 것임
            for (int k = 0; k < fileEntry_len_main && searchresult == 0; k++)
            {
                try
                {
                    //Console.WriteLine("탐색 중인 디렉토리 : " + subdirectory);
                    ValueClass.mutex.WaitOne(); //뮤텍스 락
                    writer.WriteLine("[DIRECTORY] " + fileEntry_main[k]);
                    ValueClass.mutex.ReleaseMutex();

                    string[] fileEntry = Directory.GetFiles(fileEntry_main[k]); //파일의 개수를 구함
                    int fileEntry_len = fileEntry.Length; //길이를 구함

                    for (int i = 0; i < fileEntry_len && searchresult == 0; i++) //길이만큼 안에서 파일이 있는지 탐색
                    {
                        if (fileEntry[i].Contains(ValueClass.filename))
                        {
                            searchresult = 1; //찾았으면 그만
                            ValueClass.mutex.WaitOne(); //뮤텍스 락
                            writer.WriteLine("---------------------------\n"
                                             + "찾은 파일 경로  : " + fileEntry[i]
                                             + "\n---------------------------");
                            Console.WriteLine("---------------------------\n" //ConSole.WriteLine을 세 번 호출하는 것은 비효율적이라 판단
                                             + "찾은 파일 경로  : " + fileEntry[i]
                                             + "\n---------------------------");
                         
                            ValueClass.mutex.ReleaseMutex();
                            break;
                        }
                        else
                        {
                            ValueClass.mutex.WaitOne(); //뮤텍스 락
                            writer.WriteLine("[FILE] " + fileEntry[i]);
                            ValueClass.mutex.ReleaseMutex();
                        }
                    }
                    /* 스레드를 생성하고 그 결과를 반환 */ //바로 생성하지 말고 배열에 저장했다가 한번에 생성하자.
                    if (ThreadPool_set.currentThreadcount < ValueClass.ThreadCount && searchresult == 0)
                    {
                        ++ThreadPool_set.currentThreadcount;
                        ThreadPool_set.MakeThread(fileEntry_main[k]); //스레드 풀이 다 찼는데 인자는 그대로 넘기니까 그 디렉터리는 탐색 못해서 못찾는 경우가 나는구나                                               
                        continue;
                    }


                    DirSearch(fileEntry_main[k]); //디렉터리 계속해서 재귀 탐색

                }
                catch (UnauthorizedAccessException) //권한 문제가 발생할 경우 건너띈다.
                {
                    continue;
                }
            }
        }
        catch (System.Exception excpt)
        {
            Console.WriteLine(excpt.Message);
        }
    }

    /* 로그 파일 생성 */
    public void CreateLogFile(string fileName)
    {
        string filename = "C:\\Users\\23JAE\\Desktop\\ccc\\searchLog.txt";
        string make_filename = "C:\\Users\\23JAE\\Desktop\\ccc\\searchLog"; //파일 이름을 만들기 위해 .txt를 빼줌
        int filename_counter = 1;
        try {
            while (true)
            {
                if (File.Exists(filename)) //이미 파일이 있으면
                {
                    filename = make_filename + filename_counter + ".txt";
                    filename_counter++; //없을 때 까지 진행
                }
                else
                {
                    break;
                }
            }
            writer = File.CreateText(filename);
            writer.WriteLine("탐색한 파일명 :" + fileName);
            writer.WriteLine("---------------------------");
        }
        catch (Exception e)
        {
            Console.WriteLine(e.Message);
        }
    }

    public void initProcess()
    {
        Console.WriteLine("\n--------------파일 탐색 프로그램 시작-------------\n");
        Console.Write("파일 명을 입력하세요 : ");
        ValueClass.filename = Console.ReadLine(); //파일명 입력 & 저장
        Console.WriteLine("입력한 파일명 : " + ValueClass.filename); //파일명 저장

        Console.Write("스레드 개수를 입력하세요 : ");
        get_ThreadCount_string = Console.ReadLine(); //파일명 입력 & 저장
        ValueClass.ThreadCount = int.Parse(get_ThreadCount_string); //int로 형변환

        Console.WriteLine("입력한 파일명 : " + ValueClass.filename); //파일명 저장

        p.CreateLogFile(ValueClass.filename); //기록을 남길 로그 파일 생성

        /* 탐색 시작(스레드에서) */
        Console.WriteLine("파일 검색 시작");

        ThreadPool_set.setThread(); //스레드 풀 시작
        Temp_dir_patharr = new string[ValueClass.ThreadCount];

        /* 탐색 시작 */
        search_starttime = DateTime.Now.ToLongTimeString();
        p.DirSearch(ValueClass.disk_path1);

        /* 첫 번째 디스크에서 발견 못 했을 때 다음 디스크에서 수행  (동기화 이슈)
        if (searchresult == 0)
        {
            DirSearch(ValueClass.disk_path2);
        }
        */
        try
        {
            if (searchresult == 0) /* 파일을 찾지 못했을 때 한번 재탐색 해준다.*/ //예외에서 재귀로 탐색중인 디렉터리 서치가 일반적으로 한 번 더 탐색하기 전에 끝나기 때문에
            {
                Console.WriteLine("재탐색...");
                DirSearch(ValueClass.disk_path1);
            }
            //Console.ReadLine();
            ThreadPool_set.WaitWork(); //메인이 기다리고 있는 것이기 때문에 상관 없다.
            search_endtime = DateTime.Now.ToLongTimeString();
            Console.WriteLine("탐색 시작 시간 : " + search_starttime);
            Console.WriteLine("탐색 종료 시간 : " + search_endtime);
            
            if (searchresult == 0) /* 파일을 찾지 못했을 때 */
            {
                Console.WriteLine("파일이 존재하지 않습니다.");
                Console.WriteLine("탐색 종료");
                writer.Close(); //스레드에서 종료하지 말고 여기서 종료
            }
        }
        catch (Exception e)
        {
            Console.Write("메인에서 초기화 에러 발생 원인 : ");
            Console.WriteLine(e.Message);
        }
        return;
    }


    /* 메인 함수 선언 */
    static void Main(string[] args)
    {
        p.initProcess();
        return;
    }

    /* 변수 선언 */
    static string get_ThreadCount_string;
    static int searchresult = 0;
    static StreamWriter writer; //파일 쓰기위한 스트림라이터
    static private ManualResetEvent _doneEvent;
    //static private string global_subdirs;
    //public static string[] Thread_per_searchpath = new string[ValueClass.ThreadCount]; //스레드 개수별로 할당할 디렉터리 경로명
    //public static ReaderWriterLock rwl = new ReaderWriterLock(); //쓸 때 락걸기 위한 것
    static private string search_starttime;
    static private string search_endtime;
    static public string[] Temp_dir_patharr;
    static public Boolean isException = false;
    static Program p = new Program();
}

