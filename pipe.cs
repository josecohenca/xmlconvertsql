//文向大家介绍C#实现Windows管道技术，可能好多人还不知道Windows管道技术，没有关系，看完本文你肯定有不少收获，希望本文能教会你更多东西。
//管道技术一般采用Window API来实现，最近我试着用C#实现Windows管道技术，发现C#本身方便的进程线程机制使工作变得简单至极，随手记录一下，推荐给大家。

    //首先，我们可以通过设置Process类，获取输出接口，代码如下：

Process proc = new Process();  
proc .StartInfo.FileName = strScript;  
proc .StartInfo.WorkingDirectory = strDirectory;  
proc .StartInfo.CreateNoWindow = true;  
proc .StartInfo.UseShellExecute = false;  
proc .StartInfo.RedirectStandardOutput = true;  
proc .Start();  
    
	//然后设置线程连续读取输出的字符串：

eventOutput = new AutoResetEvent(false);  
AutoResetEvent[] events = new AutoResetEvent[1];  
events[0] = m_eventOutput;  
 
m_threadOutput = new Thread( new ThreadStart( DisplayOutput ) );  
m_threadOutput.Start();  
WaitHandle.WaitAll( events );  
    
	//线程函数如下：

private void DisplayOutput()  
{  
	while ( m_procScript != null && !m_procScript.HasExited )  
	{  
		string strLine = null;  
		while ( ( strLine = m_procScript.StandardOutput.ReadLine() ) != null)  
		{  
			m_txtOutput.AppendText( strLine + "\r\n" );  
			m_txtOutputm_txtOutput.SelectionStart = m_txtOutput.Text.Length;  
			m_txtOutput.ScrollToCaret();  
		}  
		Thread.Sleep( 100 );  
	}  
	m_eventOutput.Set();  
}

 /*
 这里要注意的是，使用以下语句使TextBox显示的总是最新添加的，而AppendText而不使用+=，是因为+=会造成整个TextBox的回显使得整个显示区域闪烁

 m_txtOutput.AppendText( strLine + "\r\n" );  
 m_txtOutputm_txtOutput.SelectionStart = m_txtOutput.Text.Length;  
 m_txtOutput.ScrollToCaret();  

 为了不阻塞主线程，可以将整个过程放到一个另一个线程里就可以了，以上介绍C#实现Windows管道技术
 */