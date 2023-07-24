package cmd;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.HashMap;

//import org.json.simple.JSONObject;

import com.cfagent.api.CFAPI5J;
import com.gtone.cf.daemon.cmd.BaseCommand;
import com.gtone.cf.rt.connect.impl.AbstractConnector;
import com.gtone.cf.rt.file.FileDeployCommand;
import com.gtone.cf.rt.file.FileModel;
import com.gtone.cf.util.ICFConstants;

import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

 
 
/*
 	public static final String RCS_ID = "$Id: $";
	public final static int CMD_MANAGER_STOP = 0;			// stop manager
	public final static int CMD_AGENT_STOP = 1;				// stop agent
	public final static int CMD_MANAGER_PING = 2;			// ping maneger
	public final static int CMD_AGENT_PING = 3;				// ping agent
	public final static int CMD_VIEWDIR = 4;				// list files of directory
	public final static int CMD_MAKEDIR = 5;				// make directory
	public final static int CMD_VIEWFILE = 6;				// view file 
	public final static int CMD_CREATEFILE = 7;				// touch file
	public final static int CMD_COPYFILE = 8;				// copy file
	public final static int CMD_CHECKOUT = 9;				// checkout file
	public final static int CMD_CHECKIN = 10;				// checkin file
	public final static int CMD_REALAPPLY = 11;				// real apply
	public final static int CMD_RECOVERFILE = 12;			// recover file
	public final static int CMD_FILESIZE = 13;				// get file size
	public final static int CMD_LASTMODIFIED = 14;			// get last modified
	public final static int CMD_BUILD = 15;			   		// build
	public final static int CMD_DOSEARCH = 16;				// dir search
	public final static int CMD_DOSEARCH_AS400 = 17;		// dir search
	public final static int CMD_FTP_VIEWFILE = 18;			// ftp dir view
	public final static int CMD_FTP_UPLOAD = 19;			// ftp upload files
	public final static int CMD_FTP_DOWNLOAD = 20;			// ftp download files
	public final static int CMD_FTP_AS400_VIEWFILE = 21;	// ftp download files
	public final static int CMD_BUILD_AS400= 22;			// as400 build
	public final static int CMD_FILE_UPLOAD= 23;			// as400 build
	public final static int CMD_DOSEARCH_BIN = 24;			// dir search with bin class
	public final static int CMD_REALAPPLY_EJB = 25;			// ejb deployment
	public final static int CMD_DOSEARCH_ONLY_DIR = 26;		// dir search with bin class
	public final static int CMD_DOSEARCH_ONLY_FILE = 27;	// dir search with bin class
	public final static int CMD_DELETEFILE = 28;			// dir search with bin class
	public final static int CMD_EXECUTE = 29;				// execute command
	public final static int CMD_KILL_PROCESS = 30;			// execute command
	public final static int CMD_COPY_REMOTE_FILE = 31;		// copy remote file
	public final static int CMD_VIEWFILE_INFO = 32;			// view file with meta info
	public final static int CMD_SCANDIR = 33;				// scan file (recurse sub directory)
	public final static int CMD_DOSEARCH_ONLY_FILE_INFO = 34;	// dir search with bin class
	public final static int CMD_GET_FILES_BY_LIST = 35;		// dir search with bin class
	public final static int CMD_SVN_REVISION = 36;			// get last version
	public final static int CMD_SVN_LAST_REVISION = 37;			// get last version
	public final static int CMD_AGENT_UPDATE = 38;			// patch agent
	public final static int CMD_AGENT_RESTART = 39;			// patch restart
	public final static int CMD_SCANDIR_TO_FILE = 40;	//dir scan
	
	
	public final static String CMD_TYPE_DEPLOY = "DEPLOY";
	public final static String CMD_TYPE_BUILD = "BUILD";
	public final static String CMD_TYPE_COMMAND = "EXECUTE";
 */
public class CAgentConnector2 extends AbstractConnector {
	SimpleDateFormat sdf = new SimpleDateFormat(ICFConstants.DATE_PATTERN);
	
	public BaseCommand CMD_AGENT_PING(CFAPI5J conn, HashMap param, BaseCommand cmd) {
		boolean status=false;
		BaseCommand resultCmd = new FileDeployCommand();
		
		try {
			conn.MBRS_Run();
			
			status = conn.ReadInt()==0 ? true:false ;
			
			String msg=conn.ReadString();
			cmd.setResult(status, msg.toString()+" "+conn.brexPrimary+"/"+conn.brexPort, null);
			
			// set result
			HashMap dataHash = new HashMap();
			dataHash.put(ICFConstants.CMD_RESULT, msg.toString());
			resultCmd.setResult(true, null, dataHash);
			resultCmd.setValue(ICFConstants.CMD_RESULT,"true");
			
		} catch (Exception e) {
			resultCmd.setResult(status, e.getMessage()+" "+conn.brexPrimary+"/"+conn.brexPort, null);
			e.printStackTrace();
		}
		return resultCmd;
	} 
	
	public BaseCommand CMD_VIEWFILE(CFAPI5J conn, HashMap param, BaseCommand cmd) {
		BaseCommand resultCmd = new FileDeployCommand();
		try {
			conn.WriteString( (String) param.get("TARGET_FILE") );
			conn.MBRS_Run();
			
			boolean status = conn.ReadInt()==0 ? true:false ;
			
			if( status ) {
				String result = conn.ReadString();
				String message =conn.ReadString();
				
				if(new Boolean(result).booleanValue()) {
					String remoteTargetRootPath = (String)param.get("TARGET_PATH");
					 
					String jsonstring = conn.ReadString();
					if(jsonstring== null || "".equals(jsonstring)) {
						throw new Exception("received file info is null");
					}
					
					FileModel file = setFileModel(conn, JSONObject.fromObject(jsonstring), remoteTargetRootPath, true);
					
					resultCmd.setResult(true, null, file);
					resultCmd.setValue(ICFConstants.CMD_RESULT, "true");
				}else {
					resultCmd.setResult(false, message, null);
				}				
			}else {
				resultCmd.setResult(false, "unknown error", null);
			}
		} catch (Exception e) {
			resultCmd.setResult(false, e.getMessage()+" "+conn.brexPrimary+"/"+conn.brexPort, null);
			e.printStackTrace();
		}
		return resultCmd;
	}

	public BaseCommand CMD_DELTEFILE(CFAPI5J conn, HashMap param, BaseCommand cmd) {
		BaseCommand resultCmd = new FileDeployCommand();
		try {
			conn.WriteString( (String) param.get("TARGET_FILE") );
			conn.MBRS_Run();
			boolean status = conn.ReadInt()==0 ? true:false ;
			if( status ) {
				String result = conn.ReadString();
				String message =conn.ReadString();
				
				if(new Boolean(result).booleanValue()) {
					resultCmd.setResult(true, null, new Boolean(result));
					resultCmd.setValue(ICFConstants.CMD_RESULT, result);
				}else {
					resultCmd.setResult(false, message, null);
				}
			}else {
				resultCmd.setResult(false, "unknown error", null);
			}
  
		} catch (Exception e) {
			e.printStackTrace();
		}
		return resultCmd;
	}
	public BaseCommand CMD_CREATEFILE(CFAPI5J conn, HashMap param, BaseCommand cmd) {
		BaseCommand resultCmd = new FileDeployCommand();
		try {
			conn.WriteString( (String) param.get("TARGET_FILE") );
			conn.WriteByteFile(   (byte[])param.get("FILE_SOURCE") );
			conn.WriteString( (String) param.get("FILE_CHECKSUM") );
			conn.WriteString( (String) param.get("CHECKSUM_TYPE") );
			conn.WriteString( ""+(long) param.get("FILE_LAST_MODIFIED") );
			conn.WriteString( (String) param.get("FILE_PERMISSION") );
			
			conn.MBRS_Run();
			
			boolean status = conn.ReadInt()==0 ? true:false;
			
			if( status ) {
				String result = conn.ReadString();
				String message =conn.ReadString();
								
				if(new Boolean(result).booleanValue()) {
					resultCmd.setResult(true, null, new Boolean(result));
					resultCmd.setValue(ICFConstants.CMD_RESULT, result);
				}else {
					resultCmd.setResult(false, message, null);
				}
			}else {
				resultCmd.setResult(false, "unknown error", null);
			}
		} catch (Exception e) {
			resultCmd.setResult(false, e.getMessage()+" "+conn.brexPrimary+"/"+conn.brexPort, null);
			e.printStackTrace();
		}
		return resultCmd;
	}

	public BaseCommand CMD_DOSEARCH_ONLY_FILE(CFAPI5J conn, HashMap param, BaseCommand cmd) {
//		inHash.put("TARGET_IP", "172.16.15.15");
////		inHash.put("TARGET_IP", "127.0.0.1");
//		inHash.put("TARGET_PORT", "30502");
//		inHash.put("CONNECT_TYPE", "A");
//		inHash.put("MACHINE_TYPE", "S");
//		
//		String toFile = "/home/cf/temp/a001";
//		inHash.put("TARGET_FILE", toFile); //������ ���� ���
		boolean status=false;
		String msg=""; 
		try {
//			inHash.put("TARGET_PATH", "D:/50_INSTALL/SampleBiz/real/java"); //������ ���� ���� �ֻ��� ���
//			inHash.put("TARGET_REGEXP", "/compressionFilters/Compression[-*_*.*A-Za-z0-9]*.java"); //���Խ� 1�� ���� - Test�� �����ϴ� .html����

			
			conn.WriteString( (String) param.get("TARGET_PATH") );
			conn.WriteString( (String) param.get("TARGET_REGEXP") );
			conn.MBRS_Run();
			status = conn.ReadInt()==0 ? true:false ;
			if( status ) {
				
//				String filename=conn.ReadString();
//				conn.ReadFile((String) param.get("TARGET_PATH")+File.separator, filename );//"BB.ZIP");
				msg=conn.ReadString();
				System.out.println(msg);
				cmd.setResult(status, msg, null);  
				System.out.println(msg);
			}else {
				msg=conn.ReadString();
				System.out.println(msg);
				cmd.setResult(status, msg, null);  
//				msg=conn.ReadString();
//				cmd.setResult(status, msg.toString()+" "+conn.brexPrimary+"/"+conn.brexPort, null);  
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return cmd;
	}
	
	public BaseCommand CMD_VIEWDIR(CFAPI5J conn, HashMap param, BaseCommand cmd) {
		BaseCommand resultCmd = new FileDeployCommand();
		try {
			conn.WriteString( (String) param.get("TARGET_PATH") );
			conn.WriteString( (String) param.get("INCLUDE_SUB_DIR") );
			conn.WriteString( (String) param.get("DEFAULT_GET_ROWS") );
//			conn.WriteString( (String) param.get("TARGET_REGEXP") );
//			conn.WriteString( (String) param.get("INCLUDE_FILTER") );
//			conn.WriteString( (String) param.get("IGNORE_FILTER") );
//			conn.WriteString( (String) param.get("START_ROW") );
//			conn.WriteString( (String) param.get("DEFAULT_GET_ROWS") );
			
			conn.MBRS_Run();
			
			boolean status = conn.ReadInt()==0 ? true:false ;
			
			if( status ) {
				String result = conn.ReadString();
				String message =conn.ReadString();
				
				if(new Boolean(result).booleanValue()) {
					String remoteTargetRootPath = (String)param.get("TARGET_PATH");
					
					String jsonstring = conn.ReadLongString();
					if(jsonstring== null || "".equals(jsonstring)) {
						throw new Exception("received file info is null");
					}
					System.out.println(jsonstring);
					JSONArray viewDirList = JSONArray.fromObject(jsonstring);
					int size  = viewDirList.size();
					ArrayList resultList = new ArrayList();
					for(int i=0; i<size; i++)
					{
						resultList.add(setFileModel(conn, (JSONObject)viewDirList.get(i), remoteTargetRootPath, false));
					}
					
					resultCmd.setResult(true, null, resultList);
					resultCmd.setValue(ICFConstants.CMD_RESULT, "true");
				}else {
					resultCmd.setResult(false, message, null);
				}				
			}else {
				resultCmd.setResult(false, "unknown error", null);
			}
		} catch (Exception e) {
			resultCmd.setResult(false, e.getMessage()+" "+conn.brexPrimary+"/"+conn.brexPort, null);
			e.printStackTrace();
		}
		return resultCmd;
	}
	
	public FileModel setFileModel(CFAPI5J conn, JSONObject fileObj, String remoteTargetRootPath, boolean includeSource) throws Exception
	{
		FileModel file = new FileModel();
		
		if(fileObj.containsKey("filename")) file.setFilename(fileObj.getString("filename"));
		if(fileObj.containsKey("is_dir")) {
			file.setIsDirectory(fileObj.getInt("is_dir")==1? true: false);
			if(file.isDirectory()) {
				file.setType( "DIR" );
			}else {
				file.setType( "File" );
			}
		}
		if(fileObj.containsKey("mtime")) file.setLastModifiedDate(sdf.format(new java.util.Date(fileObj.getLong("mtime")*1000)));
		if(fileObj.containsKey("path")) {
			String filePath = fileObj.getString("path"); 
			file.setPath(filePath);					
			file.setRootPath(remoteTargetRootPath);
			file.setRelPath(filePath.replaceFirst(remoteTargetRootPath, "")); // root부터 상태 경로
		}
		 
		if(fileObj.containsKey("read")) file.setCanRead(fileObj.getInt("read")==1? true: false);
		if(fileObj.containsKey("write")) file.setCanWrite(fileObj.getInt("write")==1? true: false);
		if(fileObj.containsKey("size")) {
			file.setLength(fileObj.getLong("size"));
			file.setSize( getFileSize(fileObj.getLong("size")) );
		}
		if(fileObj.containsKey("checksum")) file.setChecksum(fileObj.getString("checksum"));
		if(includeSource) file.setFileSource(conn.ReadFileByte());
		
		return file;
	}
	public BaseCommand remoteCmdRun(HashMap globalMap, HashMap param, BaseCommand cmd) throws Exception{
		CFAPI5J conn = new CFAPI5J();
		String ip = (String)param.get(ICFConstants.TARGET_IP);
		int port = Integer.parseInt( (String)param.get(ICFConstants.TARGET_PORT));
		BaseCommand resultCmd = null;
		conn.brexPrimary=ip;
		conn.brexPort=port;
		conn.Initialize("" + cmd.getCommand()); //CMD_DELETEFILE
		switch(cmd.getCommand()) {
		case BaseCommand.CMD_AGENT_PING:
			resultCmd = CMD_AGENT_PING(conn,param,cmd);
			break;
		case BaseCommand.CMD_CREATEFILE:
			resultCmd = CMD_CREATEFILE(conn,param,cmd);
			break;
		case BaseCommand.CMD_VIEWFILE:
			resultCmd = CMD_VIEWFILE(conn,param,cmd);
			break;			
		case BaseCommand.CMD_DELETEFILE:
			resultCmd = CMD_DELTEFILE(conn,param,cmd);
			break;
		case BaseCommand.CMD_DOSEARCH_ONLY_FILE:
			resultCmd = CMD_DOSEARCH_ONLY_FILE(conn,param,cmd);
			break;
		case BaseCommand.CMD_VIEWDIR:
			resultCmd = CMD_VIEWDIR(conn,param,cmd);
			break;
		}
		return resultCmd;
	}

	@Override
	public void close(Object arg0) throws Exception {
		// TODO Auto-generated method stub
		
	}

	@Override
	public boolean isConnected(Object arg0) throws Exception {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public Object open(HashMap arg0) throws Exception {
		// TODO Auto-generated method stub
		return null;
	}
	
	protected String getFileSize(long size) throws Exception {
		try{			
			if(size == 0)
				return ICFConstants.BLANK;

			if (size > 1024)
				return (size/1024) + " KB";
			else
				return size + " B";
		}catch(Exception e){
			throw e;
		}
	}
}
