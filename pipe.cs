//�����ҽ���C#ʵ��Windows�ܵ����������ܺö��˻���֪��Windows�ܵ�������û�й�ϵ�����걾����϶��в����ջ�ϣ�������̻ܽ�����ණ����
//�ܵ�����һ�����Window API��ʵ�֣������������C#ʵ��Windows�ܵ�����������C#������Ľ����̻߳���ʹ������ü����������ּ�¼һ�£��Ƽ�����ҡ�

    //���ȣ����ǿ���ͨ������Process�࣬��ȡ����ӿڣ��������£�

Process proc = new Process();  
proc .StartInfo.FileName = strScript;  
proc .StartInfo.WorkingDirectory = strDirectory;  
proc .StartInfo.CreateNoWindow = true;  
proc .StartInfo.UseShellExecute = false;  
proc .StartInfo.RedirectStandardOutput = true;  
proc .Start();  
    
	//Ȼ�������߳�������ȡ������ַ�����

eventOutput = new AutoResetEvent(false);  
AutoResetEvent[] events = new AutoResetEvent[1];  
events[0] = m_eventOutput;  
 
m_threadOutput = new Thread( new ThreadStart( DisplayOutput ) );  
m_threadOutput.Start();  
WaitHandle.WaitAll( events );  
    
	//�̺߳������£�

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
 ����Ҫע����ǣ�ʹ���������ʹTextBox��ʾ������������ӵģ���AppendText����ʹ��+=������Ϊ+=���������TextBox�Ļ���ʹ��������ʾ������˸

 m_txtOutput.AppendText( strLine + "\r\n" );  
 m_txtOutputm_txtOutput.SelectionStart = m_txtOutput.Text.Length;  
 m_txtOutput.ScrollToCaret();  

 Ϊ�˲��������̣߳����Խ��������̷ŵ�һ����һ���߳���Ϳ����ˣ����Ͻ���C#ʵ��Windows�ܵ�����
 */