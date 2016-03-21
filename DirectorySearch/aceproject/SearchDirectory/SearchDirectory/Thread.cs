using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

/* 스레드 풀 설정 클래스 */

/*
하지만 ThreadPool로 Thread를 실행하게 되면 독립적인 스레드가 아닌 호출된 메소드 안에 상주하게 됩니다.
즉 호출된 메소드가 종료를 하게 되면 실행된 메소드들이 종료가 되겠습니다.
*/


public class ThreadPool_set
{
    static ManualResetEvent[] doneEvents;
    public static int currentThreadcount = 0; //현재 스레드 개수
    static Program p;
    public static void setThread()
    {
        /* 각각의 이벤트 스레드 할당 */
        doneEvents = new ManualResetEvent[ValueClass.ThreadCount];
    }

    public static void MakeThread(string getsubdirs) //디렉터리 경로를 인자로 전달 받음
    {
        try {
            doneEvents[currentThreadcount - 1] = new ManualResetEvent(false); //true : 이벤트의 초기 상태를 신호 상태로 설정, false : 비신호 상태로 설정
            p = new Program(doneEvents[currentThreadcount - 1]); //생성자 호출
            ThreadPool.QueueUserWorkItem(p.ThreadPoolCallback, getsubdirs); //실행을 위해 메서드를 큐에 대기시키고 메서드에서 사용할 데이터가 들어 있는
                                                                                  //개체를 지정한다. 이 메서드는 스레드 풀 스레드를 사용할 수 있을 때 실행된다.
        }
        catch (Exception)
        {
            p.DirSearch(getsubdirs); //어떠한 이유에서든 예외가 발생하면 그냥 재귀로 계속 탐색하면 된다. //여기서 찾고 있는데 메인이 종료해 버려서 생기는 에러
        }
        return;
    }


    public static void WaitWork()
    {
        /* 스레드 풀에 있는 모든 스레드가 작업을 완료할 때 까지 대기한다. */
        WaitHandle.WaitAny(doneEvents);
        //WaitHandle.WaitAll(doneEvents); //두 개가 같은데 왜 다를까
        Console.WriteLine("모든 작업이 완료되었습니다.");
    }


    static public void currentCountDiscount()
    {
        ValueClass.mutex.WaitOne(); //뮤텍스 락
        if(currentThreadcount>0)
            --currentThreadcount; //이벤트를 완료하고 종료했기 때문에 스레드를 제거해준다.
        ValueClass.mutex.ReleaseMutex();
    }
    /*
    static public void currentCountInCrease()
    {
        ValueClass.mutex.WaitOne(); //뮤텍스 락
        currentThreadcount++; //이벤트를 완료하고 종료했기 때문에 스레드를 제거해준다.
        ValueClass.mutex.ReleaseMutex();
    }


    */
}
