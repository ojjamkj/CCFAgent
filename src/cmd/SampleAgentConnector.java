package cmd;

/*
 * @(#)AgentConnector.java
 *
 * Copyright (c) 1998-2007 ITPlus Co., Ltd. All Rights Reserved.
 */

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.util.HashMap;
import java.util.Iterator;

import com.gtone.cf.daemon.cmd.BaseCommand;
import com.gtone.cf.rt.connect.impl.AbstractConnector;
//import com.gtone.cf.util.ExceptionHandler;
import com.gtone.cf.util.ICFConstants;

/**
 * CAgentConnector.java
 * 타겟시스템의 Windows/UNIX에 설치된 CAgent에 접근시 사용
 *
 */

public class SampleAgentConnector extends AbstractConnector {

	  
	
	protected BaseCommand getResultCommand(FileOutputStream fis,ObjectInputStream dataIn) throws Exception
	{
		Object obj = dataIn.readObject();
		BaseCommand resultCmd = null;
		while( obj != null )
		{
			if (obj instanceof java.lang.String) {      
				//getLogger().println(LogLevel.INFO, this.getLoggerPrefix() + "Agent CMD Result =>" + obj.toString() );
				fis.write((obj.toString()+"\n").getBytes());
	            obj = dataIn.readObject();
			}else{
				resultCmd = (BaseCommand)obj;
				break;
			}
        }
		return resultCmd;
	}
	public BaseCommand connect(BaseCommand cmd) throws Exception {
		
		 
		ObjectOutputStream dataOut = null;
		ObjectInputStream dataIn = null;
		FileOutputStream fis = null;
		BaseCommand resultCmd = null;
		try{
			 
			HashMap inHash = (HashMap)cmd.getValue(ICFConstants.HASHMAP);
			Socket m_socket=(Socket)inHash.get(Socket.class.getName());
			dataOut = new ObjectOutputStream(m_socket.getOutputStream());
			dataIn = new ObjectInputStream(m_socket.getInputStream());
			
			
			
			
			if(inHash.get(ICFConstants.EXECUTE_RESULT_FILE) != null)
			{
				fis = new FileOutputStream((String) inHash.get(ICFConstants.EXECUTE_RESULT_FILE), true);
			}
			 
			Iterator it = inHash.keySet().iterator();
			HashMap serial = new HashMap();
			while(it.hasNext())
			{
				String key = (String)it.next();
				Object obj = inHash.get(key);
				if(  obj instanceof Serializable /*&& !key.startsWith(ConnectorFactory.CONN_MAP_KEY) && !(obj instanceof jspeed.base.jdbc.CacheResultSet)*/)
				{
					serial.put(key, obj);
				}
			}
//			getLogger().println(LogLevel.INFO, this.getLoggerPrefix() + "CONNECT PARAM=" + serial);
			cmd.setValue(ICFConstants.HASHMAP, serial);
			dataOut.writeObject( cmd );
			inHash.putAll(serial);
			
			resultCmd = this.getResultCommand(fis, dataIn);
//			getLogger().println(LogLevel.INFO, this.getLoggerPrefix() + "AgentConnector: result SUCCESS ? =>" + resultCmd.isSuccess() );
		}catch(SocketTimeoutException ce){
//			ExceptionHandler.handleException(ce);
			
//			getLogger().println(LogLevel.WARNING,this.getLoggerPrefix() + getFaileMsg( ) );
			
			cmd.setResult(false, getFaileMsg( ), getFaileMsg( ));
			return cmd;
		}catch(IOException io){
//			ExceptionHandler.handleException(io);
			
//			getLogger().println(LogLevel.WARNING,this.getLoggerPrefix() + getFaileMsg( ));
			
			cmd.setResult(false, getFaileMsg( ), getFaileMsg( ));
			return cmd;
		}catch(Exception e){
//			ExceptionHandler.handleException(e);
			
//			getLogger().println(LogLevel.WARNING,this.getLoggerPrefix() + e.getMessage() );
			
			cmd.setResult(false, e.getMessage(), e.getMessage());
			return cmd;
		}finally{
			if(dataOut != null)try{dataOut.close();}catch(Exception e){}
			if(dataIn != null)try{dataIn.close();}catch(Exception e){}
       
        	if(fis != null)try{fis.close();}catch(Exception e){}
        }

		return resultCmd;
	}
	private String getFaileMsg( ){
		//return "Agent IP=" + this.getIp() + ", Port=" + this.getPort() + " is correct? Check Agent IP and port or Check Agent is alive";
//		return "Agent IP=" + this.getIp() + ", Port=" + this.getPort() + MessageHelper.getInstance().getMessage("B181", ICFConstants.LOCALE);
		return "Agent IP=" + this.getIp() + ", Port=" + this.getPort() + " 연결정보를 확인하세요.";
	}
	public void close(Object obj) throws Exception {
		Socket socket = (Socket)obj;
		socket.close();
		
	}

	public boolean isConnected(Object obj) throws Exception {
		Socket socket = (Socket)obj;
		if(socket.isClosed())
		{
			return false;
		}
		return socket.isConnected();
	}

	public Object open(HashMap inHash) throws Exception {
		HashMap iHash = new HashMap();
		iHash.put(ICFConstants.TARGET_IP, this.getIp());
//		ConnectionLicenseHelper.checkConnectionLicense(inHash);
		
		
		Socket m_socket = null;
		 
			
		m_socket = new Socket();
		m_socket.connect(
								new InetSocketAddress(this.getIp(),Integer.parseInt(this.getPort())), 
								5000);
			
		m_socket.setKeepAlive( true );
			//socket.setSoTimeout(3000);
			//socket.setTcpNoDelay(true);
		 
		return m_socket;
	}

}