package com.cfagent.api;


import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.SocketException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.Properties;
import java.util.Vector;

public class CFAPI5J {
	public  static String   brexPrimary = "172.16.15.15"; 
	public  static int      brexPort = 30502;
	private static boolean isIP= true;
	private static boolean isReadMaxParamSize = true;
	private static int     maxInOutBufferSize;  
	
	private String m_ruleId="";

	public int m_itemCnt;
	public String errorMessage;
	public int errorCode;
	public MTObjectOut OutBuffer;
	public MTObjectIn InBuffer;
	public int reOpenport = 35400;
	public int callCount = 0;
	public OutputStream out = null;
//	public static int maxParamSize = 2048000;  //1048576;
	public static int maxParamSize = 1024000000;  //1048576;
	public byte[] inByteStream = null;
	public byte[] tempStream = null;

	public String debugMessage = "";
	public static boolean isDebug = false;

	public Socket clientSock2;
	public Socket clientSock;
	public static int socketOpenCount = 0;
	public static int socketCloseCount = 0;

	public static String pEnv;
	public static int timeout = 0;
	public static int readTimeout = 0;
	public static int endFlag = -1;
	public static long    engineCheckTime=-1;

	public byte priority = 0;
	public int queue_idx = -1;
	public int maxFailCount = 1;
	public InputStream ins = null;

	public static final  boolean SERVER=true;
	public static final  boolean PORT=false;
	public static boolean failOverServer=SERVER;

	private static String resource = "/rapi4j.properties";

	private boolean isConnectTimeOut = false;
	private long    sTmie, eTime;
	public void log_write(String msg) {
		System.out.println(msg);
	}

	public long currentTimeMillis() {
		return System.currentTimeMillis();

	}

	public byte getPriority() {
		return this.priority;
	}

	public void setPriority(byte priority) {
		this.priority = priority;
	}
	public static boolean isFailOverServer() {
		return failOverServer;
	}
	public int getMaxParamSize() {
		return maxParamSize;
	}

	public static void setMaxParamSize(int _maxParamSize) {
		  maxParamSize =_maxParamSize;
	}

	public CFAPI5J() {
		maxInOutBufferSize = maxParamSize;
		this.errorCode = 0;
		this.errorMessage = new String();
		this.inByteStream = new byte[maxInOutBufferSize];

		String sConv = " ";
	}
	public CFAPI5J(String ip,int port,String conv) {
		super();
		brexPrimary=ip;
		brexPort=port;
	}

	public CFAPI5J(String conv) {
		maxInOutBufferSize = maxParamSize;
		this.errorCode = 0;
		this.errorMessage = new String();
		this.inByteStream = new byte[maxInOutBufferSize];
	}

	public CFAPI5J(String conv, String _startPoint, String aVersion) {
		maxInOutBufferSize = maxParamSize;
		this.errorCode = 0;
		this.errorMessage = new String();
		this.inByteStream = new byte[maxInOutBufferSize];
	}

	public CFAPI5J(String conv, int bufsize) {
		maxInOutBufferSize = CFAPI5J.maxParamSize = bufsize;
		this.errorCode = 0;
		this.errorMessage = new String();
		this.inByteStream = new byte[maxInOutBufferSize];
	}

	public CFAPI5J(String conv, String _startPoint) {
		maxInOutBufferSize = maxParamSize;
		this.errorCode = 0;
		this.errorMessage = new String();
		this.inByteStream = new byte[maxInOutBufferSize];
	}

	public void initlog(boolean isLog, String msg) {
		if (isLog) {
			System.out.print(msg);
		}
	}

	public static synchronized String DGetEnv(String name) {
		String rValue = "";
		try {
			rValue = System.getProperty(name);
		}catch (NullPointerException e) {
			rValue="";
		} catch (Exception e) {
			e.printStackTrace();
		}
		return rValue;
	}
  public static synchronized String GetEnvString(String name, String defaultVale ){
  	String msg="BREX INIT -> System.getProperty Read";
		String pEnv2 = DGetEnv(name);
		if ((pEnv2 != null) && (!pEnv2.trim().equals("NULL")) && (!pEnv2.trim().equals(""))) {
		}else{
			pEnv2=defaultVale;
		}
		if (isDebug) {
			System.out.println(msg+":["+name+"]=>[" + pEnv2.trim() + "]");
		}
		return pEnv2;
  }
  public static synchronized int GetEnvInt(String name, int defaultVale){
  	String msg="BREX INIT -> System.getProperty Read";
		String pEnv2 = DGetEnv(name);
		int val=0;
		if ((pEnv2 != null) && (!pEnv2.trim().equals("NULL")) && (!pEnv2.trim().equals(""))) {
				try{
					val= Integer.parseInt(pEnv2.trim());
				}catch (Exception e) {
					val=defaultVale;
				}
 
		}else{
			val= defaultVale;
		}
		if (isDebug) {
			System.out.println(msg+":["+name+"]=>[" + val+ "]");
		}
		return val;
  }
  public static synchronized String GetPropertiesString(Properties p,String name, String defaultVale ){
  	String msg="BREX INIT -> Properties Read";
  	String pEnv2="";
			if (p.containsKey( name )) {
				pEnv2 = p.getProperty(name );
				if ((pEnv2 != null) && (!pEnv2.trim().equals("NULL")) && (!pEnv2.trim().equals(""))) {
				}else{
					pEnv2=defaultVale;
				}
			}else{
				pEnv2=defaultVale;
			}
		if (isDebug) {
			System.out.println(msg+":["+name+"]=>[" + pEnv2 + "]");
		}
		return pEnv2;
  }

  public static synchronized int GetPropertiesInt(Properties p,String name, int defaultVale){
  	String msg="BREX INIT -> Properties Read";
  	String pEnv2="";
		int val=0;
			if (p.containsKey( name )) {
				pEnv2 = p.getProperty(name );
				if ((pEnv2 != null) && (!pEnv2.trim().equals("NULL")) && (!pEnv2.trim().equals(""))) {
					try {
						val = Integer.parseInt(pEnv2.trim());
					} catch (Exception e) {
						val=defaultVale;
					}
				}else{
					val=defaultVale;
				}
			}else{
				val=defaultVale;
			}
		if (isDebug) {
			System.out.println(msg+":["+name+"]=>[" + val+ "]");
		}
		return val;
  }
  
  

	public long getEngineCheckTime() {
		return engineCheckTime;
	}

	public MTObjectIn getInBuffer() {
		return this.InBuffer;
	}

	public MTObjectOut getOutBuffer() {
		return this.OutBuffer;
	}

	public static boolean isIP() {
		return isIP;
	}

	public long BuildResult(byte[] inOutBuffer) throws Exception {
		Object resultValue = null;
		try {
			this.InBuffer = new MTObjectIn(inOutBuffer);
		} catch (Exception e) {
			this.errorCode = -1;
			this.errorMessage = e.toString();
			return this.errorCode;
		}
		long rc = this.InBuffer.ReadInt();
		if (rc != 0L) {
			this.errorCode = this.InBuffer.ReadInt();
			this.errorMessage = this.InBuffer.ReadString();
			return this.errorCode;
		}
		return 0L;
	}

	public void AddItemCount(int cnt) {
		this.OutBuffer.WriteInt(cnt);
		if (isDebug) {
			this.debugMessage = (this.debugMessage + "\t AddItemCount(" + cnt + ")");
		}
	}

	public void AddString(String dblVal) {
		this.OutBuffer.WriteInt(1);
		this.OutBuffer.WriteString(dblVal);
		if (isDebug) {
			this.debugMessage = (this.debugMessage + "\t AddString(" + dblVal + ")");
		}
	}

	public void AddStringArray(String dblVal[]) {
		if (dblVal == null || dblVal.length <= 0) {
			this.OutBuffer.WriteInt(1);
			this.OutBuffer.WriteString("");
		} else {
			this.OutBuffer.WriteInt(dblVal.length);
			for (int i = 0; i < dblVal.length; i++) {
				this.OutBuffer.WriteString(dblVal[i]);
			}
		}
	}
 
	public void WriteString(String dblVal) {
		this.OutBuffer.WriteString(dblVal);
		if (isDebug) {
			this.debugMessage = (this.debugMessage + "\t WriteString(" + dblVal + ")");
		}
	}
	public void WriteByteFile(byte byteVal[]) {
		this.OutBuffer.WriteByteFile(byteVal);
		if (isDebug) {
			this.debugMessage = (this.debugMessage + "\t WriteString(" + byteVal + ")");
		}
	}
	public void WriteByte(byte dblVal[]) {
		this.OutBuffer.WriteByte(dblVal);
		if (isDebug) {
			this.debugMessage = (this.debugMessage + "\t WriteString(" + dblVal + ")");
		}
	}

	public void AddItemString(String dblVal) {
		this.OutBuffer.WriteString(dblVal);
		if (isDebug) {
			this.debugMessage = (this.debugMessage + "\t AddItemString(" + dblVal + ")");
		}
	}
 

	public void AddInt(int dblVal) {
		this.OutBuffer.WriteInt(1);
		this.OutBuffer.WriteInt(dblVal);
		if (isDebug) {
			this.debugMessage = (this.debugMessage + "\t AddInt(" + dblVal + ")");
		}
	}

	public void AddIntArray(int dblVal[]) {
		if (dblVal == null || dblVal.length <= 0) {
			this.OutBuffer.WriteInt(1);
			this.OutBuffer.WriteInt(0);
		} else {
			this.OutBuffer.WriteInt(dblVal.length);
			for (int i = 0; i < dblVal.length; i++) {
				this.OutBuffer.WriteInt(dblVal[i]);
			}
		}
	}

	public void WriteInt(int dblVal) {
		this.OutBuffer.WriteInt(dblVal);
		if (isDebug) {
			this.debugMessage = (this.debugMessage + "\t WriteInt(" + dblVal + ")");
		}
	}

	public void WriteFraction(double dblVal) {
		this.OutBuffer.WriteString("" + dblVal);
		if (isDebug) {
			this.debugMessage = (this.debugMessage + "\t WriteInt(" + dblVal + ")");
		}
	}

	public void AddItemInt(int dblVal) {
		this.OutBuffer.WriteInt(dblVal);
		if (isDebug) {
			this.debugMessage = (this.debugMessage + "\t AddItemInt(" + dblVal + ")");
		}
	}

	public void AddItemFraction(double dblVal) {
		double dblVal2;
		try {
			dblVal2 = dblVal;
		} catch (Exception e) {
			dblVal2 = 0.0D;
		}
		this.OutBuffer.WriteString("" + dblVal2);
		if (isDebug) {
			this.debugMessage = (this.debugMessage + "\t AddItemFraction(" + dblVal + ")");
		}
	}

	public void AddFraction(double dblVal) {
		double dblVal2;
		try {
			dblVal2 = dblVal;
		} catch (Exception e) {
			dblVal2 = 0.0D;
		}
		this.OutBuffer.WriteInt(1);
		this.OutBuffer.WriteString("" + dblVal2);
		if (isDebug) {
			this.debugMessage = (this.debugMessage + "\t AddFraction(" + dblVal2 + ")");
		}
	}

	public void AddFractionArray(double dblVal[]) {
		double dblVal2;

		if (dblVal == null || dblVal.length <= 0) {
			this.OutBuffer.WriteInt(1);
			this.OutBuffer.WriteString("" + 0.0D);
		} else {
			this.OutBuffer.WriteInt(dblVal.length);
			for (int i = 0; i < dblVal.length; i++) {
				try {
					dblVal2 = dblVal[i];
				} catch (Exception e) {
					dblVal2 = 0.0D;
				}
				this.OutBuffer.WriteString("" + dblVal2);
			}
		}
	}

	public String ReadString() {
		return getInBuffer().ReadString();
	}
	public String ReadLongString() {
		return getInBuffer().ReadLongString();
	}
	public String ReadFile(String path, String name) {
		return getInBuffer().ReadFile(path, name);
	}
	public byte[] ReadFileByte() {
		return getInBuffer().ReadFileByte();
	}
	public String ReadString(String charsetName) {
		return getInBuffer().ReadString(charsetName);
	}

	public double ReadFraction() {
		return getInBuffer().ReadDouble();
	}

	public double ReadDouble() {
		return getInBuffer().ReadDouble();
	}

	public byte ReadByte() {
		return getInBuffer().ReadByte();
	}

	public int ReadInt() {
		return getInBuffer().ReadInt();
	}

	private boolean DateConvertL2S(String strDate) {
		strDate = strDate.substring(0, 4) + strDate.substring(5, 7) + strDate.substring(8, 10);
		return true;
	}

	private String DateConvertS2L(String strDate) {
		return strDate.substring(0, 4) + "-" + strDate.substring(4, 6) + "-" + strDate.substring(6, 8);
	}

	private boolean DateTypeCheck(String strDate) {
		String descDate = strDate.trim();
		if (strDate.length() == 8) {
			descDate = DateConvertS2L(strDate);
		} else {
			char[] pszDate = new char[11];
			strDate.getChars(0, 10, pszDate, 0);
			if ((pszDate[4] != '-') || (pszDate[7] != '-')) {
				return false;
			}
		}
		int yy = new Integer(descDate.substring(0, 4)).intValue();
		if ((yy < 1900) || (yy > 2999)) {
			return false;
		}
		int mm = new Integer(descDate.substring(5, 7)).intValue();
		int dd = new Integer(descDate.substring(8, 10)).intValue();
		if ((mm < 1) || (mm > 12)) {
			return false;
		}
		if ((dd < 1) || (dd > LastDayOfMonth(yy, mm))) {
			return false;
		}
		strDate = descDate;
		return true;
	}

	public int getErrorCode() {
		return this.errorCode;
	}

	public String getErrorMessage() {
		return this.errorMessage;
	}

	public void Clear() {
		if (this.OutBuffer != null) {
			this.OutBuffer.Clear();
		}
		this.OutBuffer.WriteString(this.m_ruleId);
		this.OutBuffer.WriteInt(0);
	}

	public void Initialize(String ruleId, int cnt, byte[] ItemTypes, int[] ItemCd) {
		this.m_ruleId = ruleId;
		this.m_itemCnt = cnt;
		if (this.OutBuffer != null) {
			this.OutBuffer.Clear();
		}
		this.OutBuffer = new MTObjectOut( maxParamSize);
		this.OutBuffer.WriteString(this.m_ruleId);
		this.OutBuffer.WriteInt(0);
		this.OutBuffer.WriteByte(this.m_itemCnt);
		for (int i = 0; i < this.m_itemCnt; i++) {
			this.OutBuffer.WriteInt(ItemCd[i]);
			this.OutBuffer.WriteByte(ItemTypes[i]);
		}
		if (isDebug) {
			this.debugMessage = "";
		}
	}

	public void Initialize(String ruleId) {
		this.m_ruleId = ruleId;
		this.OutBuffer = new MTObjectOut( maxParamSize);
		this.OutBuffer.WriteString(this.m_ruleId);
		this.OutBuffer.WriteInt(0);
		if (isDebug) {
			this.debugMessage = "";
		}
	}

	private long LastDayOfMonth(long lYear, long lMonth) {
		if ((lMonth == 4L) || (lMonth == 6L) || (lMonth == 9L) || (lMonth == 11L)) {
			return 30L;
		}
		if (lMonth == 2L) {
			if (lYear % 400L == 0L) {
				return 29L;
			}
			if (lYear % 100L == 0L) {
				return 28L;
			}
			return lYear % 4L != 0L ? 28L : 29L;
		}
		return 31L;
	}
	private long BuildSendHeader(MTObjectOut ObjHeader, int sendSize, byte paramMode) {
		// if (apiVersion.trim().equals("5.0"))

		String str=""+sendSize;
	    byte[] bytes = new byte[30];
	    byte[] strBytes = str.getBytes();
	    System.arraycopy(strBytes, 0, bytes, 0, Math.min(strBytes.length, bytes.length));

	    for (int i = 0; i < 30; i++) {
			ObjHeader.WriteByte(bytes[i]);
		}
	    for (int i = 0; i < 20; i++) {
			ObjHeader.WriteByte(0);
		}
		return 0L;
	}

	private long BuildSendHeaderFull(MTObjectOut ObjHeader, int sendSize, byte paramMode, int param_qid) {
		
		String str=""+sendSize;
	    byte[] bytes = new byte[30];
	    byte[] strBytes = str.getBytes();
	    System.arraycopy(strBytes, 0, bytes, 0, Math.min(strBytes.length, bytes.length));

	    for (int i = 0; i < 30; i++) {
			ObjHeader.WriteByte(bytes[i]);
		}
	    for (int i = 0; i < 20; i++) {
			ObjHeader.WriteByte(0);
		}
		return 0L;
	}
 
	public String getFillCharacter(String data, int maxSize) {
		byte[] a = new byte[maxSize];
		if (data != null) {
			byte[] b = data.getBytes();
			if (b.length >= maxSize) {
				for (int i = 0; i < maxSize; i++) {
					a[i] = b[i];
				}
			} else {
				for (int i = 0; i < b.length; i++) {
					a[i] = b[i];
				}
				for (int i = b.length; i < maxSize; i++) {
					a[i] = 32;
				}
			}
		} else {
			for (int i = 0; i < maxSize; i++) {
				a[i] = 32;
			}
		}
		return new String(a).toString();
	}

	private int CheckReceiveHeader(Socket _clientSocket) {
		MTObjectIn InHeader = null;
		long start, end1, end2, end3, end4;
		try {

			if (this.inByteStream == null) {
				this.inByteStream = new byte[maxInOutBufferSize];
			} else {
				this.inByteStream = null;
				this.inByteStream = new byte[maxInOutBufferSize];
			}
			start = currentTimeMillis();
			if (readByteStream(_clientSocket, 50) != 0)
				return -1;
			end1 = currentTimeMillis();
			InHeader = new MTObjectIn(this.inByteStream);
		} catch (Exception e) {
			e.printStackTrace();
			if (InHeader != null) {
				InHeader = null;
			}
			this.errorMessage = e.toString();
			return -1;
		}
		int[] buffer = new int[4];
		buffer[0] = (InHeader.ReadByte() & 0xFF);
		buffer[1] = (InHeader.ReadByte() & 0xFF);
		buffer[2] = (InHeader.ReadByte() & 0xFF);
		buffer[3] = (InHeader.ReadByte() & 0xFF);
		long dataLength = (buffer[0] * 65536*256) + (buffer[1] * 256*256 ) + (buffer[2] * 256 )+ buffer[3];
		if (InHeader != null) {
			InHeader = null;
		}
		return (int) dataLength;
	}
  
	public static long getDateTime() {
		return new Date().getTime();
	}

	private synchronized int Calling(int svrIndex, int portIndex, boolean errmsg) {
		try {
System.out.println("Calling:"+brexPrimary+":"+brexPort);			
			this.clientSock = OpenSocket();
			if( clientSock!=null)return 0;
			else return -1;
		}catch(Exception e) {
			e.printStackTrace();
		}
		return -1;
	}
	public synchronized int MBRS_conn()throws Exception{
		int status=-1;
		int index;
		for ( int j=0; j <10 ; j++ ){ 
			status = Calling(j,0, false);
			if (status==0) return 0;
		}
		return -1;
	}
	public synchronized void MBRS_Run( )throws Exception { 
		try{
	  		MBRS_Call_Full(2); 
		}catch (Exception e) {
			e.printStackTrace();
			throw e;  
		}
	}
   

	public synchronized boolean MBRS_Call_Full(int parmMode) throws Exception {
		int rc = 0;
		int rcvSize = 0;
		int sendSize = 0;
		OutputStream tout = null;
		long time1=0, time2=0, time3=0;
		if( isDebug ) {
			time1=System.currentTimeMillis();
		}
		if (MBRS_conn() != 0L) {
			socketClose(0);
			throw new Exception(this.errorMessage);
			// return false;
		}
		if( isDebug ) {
			time2=System.currentTimeMillis();
			System.out.println("\tConnection:Time ("+(time2-time1 )+" )");
		}
		sendSize = this.OutBuffer.getLength();
		MTObjectOut OutHeader = new MTObjectOut();
		BuildSendHeaderFull(OutHeader, sendSize, (byte) parmMode, 0);
		// end1 = currentTimeMillis();
		SocketAddress saddr = null;
		try {
			tout = this.clientSock.getOutputStream();
			byte[] newbuf = new byte[sendSize + 50];

			System.arraycopy(OutHeader.getM_buffer(), 0, newbuf, 0, 50);
			System.arraycopy(this.OutBuffer.getM_buffer(), 0, newbuf, 50, sendSize);

			tout.write(newbuf, 0, sendSize + 50);
			tout.flush();  
			newbuf = (byte[]) null;
 
		} catch (IOException e) {
			this.errorCode = -1;
			this.errorMessage = ("Send error! " + e.toString());
			socketClose(1);
			if (OutHeader != null) {
				OutHeader = null;
			}
			throw new Exception(this.errorMessage);
		} catch (Exception e) {
			this.errorCode = -1;
			this.errorMessage = ("Send error! " + e.toString());
			socketClose(2);
			if (OutHeader != null) {
				OutHeader = null;
			}
			throw new Exception(this.errorMessage);
		}
		// end2 = currentTimeMillis();
		try {
			rcvSize = CheckReceiveHeader(this.clientSock);
//System.out.println("rcvSize(01)="+rcvSize);			
			// new Thread().sleep(10000);
			// end3 = currentTimeMillis();
			if (rcvSize < 0) {
				socketClose(3);
				if (OutHeader != null) {
					OutHeader = null;
				}
				throw new Exception(this.errorMessage);
			}
			
		}catch(Exception e) {
			e.printStackTrace();
		}
		try {
			rcvSize = (int) BuildResult(this.clientSock, rcvSize);
			// end4 = currentTimeMillis();
			if (rcvSize != 0) {
//				socketClose(4);
				throw new Exception(this.errorMessage);
			}
			return true;
		}catch(Exception e) {
			
			return false;
		}finally {
			socketClose(5);
			if( isDebug ) {
				time3=System.currentTimeMillis();
				System.out.println("\tConnection:Time ("+(time3-time1 )+" )");
			}			
		}

	}
	public synchronized void socketClose(int index) {
		if (clientSock != null) {
			try {
				clientSock.setSoLinger(true, 0);
				clientSock.setSoTimeout(0);
				clientSock.shutdownInput();
				clientSock.shutdownOutput();
				clientSock.close();
			} catch (Exception localException) {
			} finally {
				this.clientSock = null;
				socketCloseCount += 1;
			}
		}
	}

	public synchronized void socketClose(Socket sock) {
		if (sock != null) {
			try {
				sock.setSoLinger(true, 0);
				sock.setSoTimeout(0);
				sock.shutdownInput();
				sock.shutdownOutput();
				sock.close();
			} catch (Exception localException) {
			} finally {
				sock = null;
				socketCloseCount += 1;
			}
		}
	}

	public static int getSocketOpenCount() {
		return socketOpenCount;
	}

	public static int getSocketCloseCount() {
		return socketCloseCount;
	}

	public synchronized Socket OpenSocket() throws Exception {
		long start, end;
		socketOpenCount += 1;
		Socket clientSocktemp = null;
		 try{
				if (isConnectTimeOut) {
					SocketAddress socketAddress = new InetSocketAddress(brexPrimary, brexPort);
					if (null != socketAddress) {
						clientSocktemp = new Socket( );
						if (null != clientSocktemp) {
							clientSocktemp.setSoLinger(true, 0);
							clientSocktemp.connect(socketAddress, timeout);
							clientSocktemp.setSoTimeout(readTimeout);
						}else{
							clientSocktemp=null; 
						}
					}
				} else {
					clientSocktemp = new Socket(brexPrimary, brexPort);
					if (null != clientSocktemp) {
						clientSocktemp.setSoLinger(true, 0);
						clientSocktemp.setSoTimeout(readTimeout);
					}else{
						clientSocktemp=null;
					}
				}
				socketOpenCount += 1;
		 }catch (Exception e) {
				clientSocktemp=null;
		}finally{
			if( clientSocktemp!=null){
				ins = clientSocktemp.getInputStream();
			}
			return clientSocktemp;
		}
	}
	
	public synchronized Socket OpenSocketABL(String Ip, int port) throws Exception {
		long start, end;
		socketOpenCount += 1;
		Socket clientSocktemp = null;
		System.out.println("["+Ip+"]+["+port+"]");
		 try{
				if (isConnectTimeOut) {
					SocketAddress socketAddress = new InetSocketAddress(Ip, port);
					if (null != socketAddress) {
						clientSocktemp = new Socket( );
						if (null != clientSocktemp) {
							clientSocktemp.setSoLinger(true, 0);
							clientSocktemp.connect(socketAddress, timeout);
							clientSocktemp.setSoTimeout(readTimeout);
						}else{
							clientSocktemp=null;
						}
					}
					
				} else {
					clientSocktemp = new Socket(Ip, port);
					if (null != clientSocktemp) {
						clientSocktemp.setSoLinger(true, 0);
						clientSocktemp.setSoTimeout(readTimeout);
					}else{
						clientSocktemp=null;
					}
				}

				socketOpenCount += 1;
		 }catch (Exception e) {
				clientSocktemp=null;
				System.out.println("IP["+Ip+":"+port+"]"+e.getMessage());
//				e.printStackTrace();
			// TODO: handle exception
		}finally{
			if( clientSocktemp!=null){
				ins = clientSocktemp.getInputStream();
			}
			return clientSocktemp;
		}
		 
	}
	 
	public synchronized boolean MBRS_Call_Socket(int parmMode) throws Exception {
		int rc = 0;
		int rcvSize = 0;
		int sendSize = 0;
//		long start, end1, end2, end3, end4;
//		start = currentTimeMillis();

		sendSize = this.OutBuffer.getLength();
		MTObjectOut OutHeader = new MTObjectOut();
		BuildSendHeader(OutHeader, sendSize, (byte) parmMode);
//		end1 = currentTimeMillis();
		SocketAddress saddr = null;
		try {
			OutputStream out = this.clientSock.getOutputStream();
			byte[] newbuf = new byte[sendSize + 50];

			System.arraycopy(OutHeader.getM_buffer(), 0, newbuf, 0, 50);
			System.arraycopy(this.OutBuffer.getM_buffer(), 0, newbuf, 50, sendSize);

			out.write(newbuf, 0, sendSize + 50);
			out.flush();
			newbuf = (byte[]) null;
		} catch (IOException e) {
			this.errorCode = -1;
			this.errorMessage = ("Send error! " + e.toString());
			if (OutHeader != null) {
				OutHeader = null;
			}
			throw new Exception(this.errorMessage);
		} catch (Exception e) {
			this.errorCode = -1;
			this.errorMessage = ("Send error! " + e.toString());
			if (OutHeader != null) {
				OutHeader = null;
			}
			throw new Exception(this.errorMessage);
		}
//		end2 = currentTimeMillis();
		rcvSize = CheckReceiveHeader(this.clientSock);
//		end3 = currentTimeMillis();
		if (rcvSize < 0) {
			if (OutHeader != null) {
				OutHeader = null;
			}
			throw new Exception(this.errorMessage);
		}
		rcvSize = (int) BuildResult(this.clientSock, rcvSize);
//		end4 = currentTimeMillis();
		if (rcvSize != 0) {
			throw new Exception(this.errorMessage);
		}
//		System.out.println("\t\t\t\t" + (end1 - start) + "\t" + (end2 - start) + "\t" + (end3 - start) + "\t" + (end4 - start) + "\t");
		return true;
	}

	private long readByteStream(Socket _clientSocket, int rcvSize) {
		long start, end1;
		try {
			_clientSocket.setReceiveBufferSize(rcvSize);
			if (rcvSize > this.inByteStream.length) {
				byte[] newInByteStream = new byte[rcvSize];
				this.inByteStream = newInByteStream;
			}
			start = currentTimeMillis();
			int leng = ins.read(this.inByteStream, 0, rcvSize);
			end1 = currentTimeMillis();
			if (leng < 0) {
				this.errorCode = -1;
				this.errorMessage = ("Socket Read(" + leng + ") Error(BuildResult) rcvSize > " + rcvSize);
				if (this.tempStream != null) {
					this.tempStream = null;
				}
				return -1L;
			}
			if (leng != rcvSize) {
				int tempLeng = 0;
				for (int lcnt = 0;; lcnt++) {
					this.tempStream = null;
					this.tempStream = new byte[rcvSize];
					tempLeng = ins.read(this.tempStream, 0, rcvSize);
					if (tempLeng > 0) {
						if (tempLeng + leng > this.inByteStream.length) {
							byte[] newInByteStream = new byte[tempLeng + leng];
							System.arraycopy(this.inByteStream, 0, newInByteStream, 0, this.inByteStream.length);
							this.inByteStream = newInByteStream;
						}
						System.arraycopy(this.tempStream, 0, this.inByteStream, leng, tempLeng);
						leng += tempLeng;
					}
					if (this.tempStream != null) {
						this.tempStream = null;
					}
					if (leng >= rcvSize) {
						break;
					}
				}
			}
			return 0;
		} catch (IOException e) {
			this.errorCode = -1;
			this.errorMessage = e.toString();
			return errorCode;
			// TODO: handle exception
		} finally {

		}
	}

	private long BuildResult(Socket _clientSocket, int rcvSize) throws Exception {
		Object resultValue = null;

		if (readByteStream(_clientSocket, rcvSize) != 0)
			return -1;

		this.InBuffer = new MTObjectIn(this.inByteStream);
		long rc;
		if ((rc = this.InBuffer.ReadInt()) != 0L) {
			this.errorCode = this.InBuffer.ReadInt();
			this.errorMessage = this.InBuffer.ReadString();

			System.out.println("rc:" + rc + ":errorMessage::" + this.errorMessage);
			return this.errorCode;
		}
		return 0L;
	}

	public void closeSocket() {
		if (this.clientSock2 != null) {
			try {
				this.clientSock2.close();
				this.clientSock2 = null;
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	public boolean open(String mConv, String severIp, int serverPort) throws Exception {
		try {
			if (this.clientSock2 != null) {
				closeSocket();
			}
			this.clientSock2 = OpenSocket();
		} catch (Exception e) {
			this.errorCode = -1;
			this.errorMessage = e.toString();
			throw e;
		}
		this.out = this.clientSock2.getOutputStream();
		return true;
	} 

 
 
	public synchronized void run(int parmMode) throws Exception {
		MTObjectOut OutHeader = new MTObjectOut(0);
		int sendSize = this.OutBuffer.getLength(); 
		BuildSendHeader(OutHeader, sendSize, (byte) parmMode);
		byte[] newbuf = new byte[sendSize + 50];
		System.arraycopy(OutHeader.getM_buffer(), 0, newbuf, 0, 50);
		System.arraycopy(this.OutBuffer.getM_buffer(), 0, newbuf, 50, sendSize);

		this.out.write(newbuf, 0, sendSize + 50);

		this.out.flush();
		newbuf = (byte[]) null;

		int rcvSize = CheckReceiveHeader(this.clientSock2);
		if (rcvSize < 0) {
			this.errorCode = -1;
			if (this.errorMessage == null) {
				this.errorMessage = "Receive Header Not found ";
			}
			if (OutHeader != null) {
				OutHeader = null;
			}
			System.out.println(this.errorMessage);
			throw new Exception(this.errorMessage);
		}
		rcvSize = (int) BuildResult(this.clientSock2, rcvSize);
		if (rcvSize != 0) {
			this.errorCode = -1;
			if (this.errorMessage == null) {
				this.errorMessage = "Receive Data Not found ";
			}
			throw new Exception(this.errorMessage);
		}
	} 

	public void close() {
		if (this.OutBuffer != null) {
			this.OutBuffer = null;
		}
		if (this.InBuffer != null) {
			this.InBuffer = null;
		}
		if (this.tempStream != null) {
			this.tempStream = null;
		}
		if (this.clientSock2 != null) {
			try {
				this.clientSock2.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
   
	public static String getShortDateStringKey() {
		SimpleDateFormat simpledateformat = new SimpleDateFormat("yyyy-MM-dd aa  hh:mm:ss", Locale.KOREAN);
		return simpledateformat.format(new Date());
	}

	private void putSysMsg(int parmMode, Exception e) {
		StringBuffer msg = new StringBuffer();
		msg.append("BREError=>" + getShortDateStringKey() + ">> PGMID(" + this.m_ruleId + ") MODE(" + parmMode + ")");
		msg.append(">>INPUT :" + this.debugMessage + "\n");
		msg.append(">>MSG(" + this.errorCode + "::" + this.errorMessage.trim() + ")");
		if (e != null) {
			e.printStackTrace();
		}
		System.err.println(msg.toString());
	}

	public int getMaxInOutBufferSize() {
		return maxInOutBufferSize;
	}

	public void setMaxInOutBufferSize(int maxBufferSize) {
		maxInOutBufferSize = maxBufferSize;
	}

	public byte[] getDataSpace(String msg, int maxSize) {
		byte[] rValue = new byte[maxSize];
		if (msg.length() >= maxSize) {
			rValue = msg.substring(0, maxSize).getBytes();
		} else if (msg != null) {
			System.arraycopy(msg.getBytes(), 0, rValue, 0, msg.length());
			for (int i = msg.length(); i < maxSize; i++) {
				rValue[i] = 0;
			}
		} else {
			for (int i = 0; i < maxSize; i++) {
				rValue[i] = 0;
			}
		}
		return rValue;
	}
   
	public static void main(String args[]){
		try{
			if( args.length>0){
				 InputStream is =null;
				 CFAPI5J rj = new CFAPI5J();
			   is = rj.getClass().getResourceAsStream("/version.txt");
			   if( args.length>=2){
						if( args[0].trim().toLowerCase().equals("-v") && args[1].trim().toLowerCase().equals("-all") ){
						   BufferedReader reader = new BufferedReader(new InputStreamReader(is));
				        StringBuilder out = new StringBuilder();
				        String line;
				        while ((line = reader.readLine()) != null) {
			            out.append(line);
			            out.append("\n");
				        }
				        System.out.println(out.toString());   //Prints the string content read from input stream
				        reader.close();
						}else 	if( args[0].trim().toLowerCase().equals("-v") ){
						   BufferedReader reader = new BufferedReader(new InputStreamReader(is));
				        StringBuilder out = new StringBuilder();
				        String line= reader.readLine();
				        line= reader.readLine();
		            out.append(line);
				        System.out.println(out.toString().substring(0,11));   //Prints the string content read from input stream
				        reader.close();
						}
					}else{
					   BufferedReader reader = new BufferedReader(new InputStreamReader(is));
			        StringBuilder out = new StringBuilder();
			        String line= reader.readLine();
			        line= reader.readLine();
	           out.append(line);
			        System.out.println(out.toString().substring(0,11));   //Prints the string content read from input stream
			        reader.close();;
					}			
			}
		}catch (Exception e) {
			e.printStackTrace();
			// TODO: handle exception
		}
	}
}
