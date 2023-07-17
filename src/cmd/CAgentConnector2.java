package cmd;

import java.io.File;
import java.util.HashMap;

//import org.json.simple.JSONObject;

import com.cfagent.api.CFAPI5J;
import com.gtone.cf.daemon.cmd.BaseCommand;
import com.gtone.cf.rt.file.FileModel;
import com.gtone.cf.util.ICFConstants;

 
 
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
public class CAgentConnector2 {
	public BaseCommand CMD_AGENT_PING(CFAPI5J conn, HashMap param, BaseCommand cmd) {
		boolean status=false;
		String msg="";
		try {
			conn.WriteString("aaaaaaaaaaa");
			conn.MBRS_Run();
			
			status = conn.ReadInt()==0 ? true:false ;
			msg=conn.ReadString();
			cmd.setResult(status, msg.toString()+" "+conn.brexPrimary+"/"+conn.brexPort, null);  
		} catch (Exception e) {
			cmd.setResult(status, e.getMessage()+" "+conn.brexPrimary+"/"+conn.brexPort, null);  
			e.printStackTrace();
		}
		System.out.println("01:"+cmd.toString());
		return cmd;
	} 
	
	public BaseCommand CMD_VIEWFILE(CFAPI5J conn, HashMap param, BaseCommand cmd) {
		boolean status=false;//CMD_VIEWFILE
		String msg="";
		try {
			conn.WriteString( (String) param.get("TARGET_FILE") );
			conn.MBRS_Run();
			status = conn.ReadInt()==0 ? true:false ;
			if( status ) {
				String filename=conn.ReadString();
				conn.ReadFile((String) param.get("TARGET_PATH")+File.separator, filename );//"BB.ZIP");
				cmd.setResult(status, " "+conn.brexPrimary+"/"+conn.brexPort, null);  
			}else {
				msg=conn.ReadString(); 
				cmd.setResult(status, msg.toString()+" "+conn.brexPrimary+"/"+conn.brexPort, null);  
			}
  
		} catch (Exception e) {
			e.printStackTrace();
		}
		return cmd;
	}

	public BaseCommand CMD_DELTEFILE(CFAPI5J conn, HashMap param, BaseCommand cmd) {
		boolean status=false;//CMD_VIEWFILE
		String msg="";
		try {
			conn.WriteString( (String) param.get("TARGET_FILE") );
			conn.MBRS_Run();
			status = conn.ReadInt()==0 ? true:false ;
			if( status ) {
				String filename=conn.ReadString();
				conn.ReadFile((String) param.get("TARGET_PATH")+File.separator, filename );//"BB.ZIP");
				cmd.setResult(status, " "+conn.brexPrimary+"/"+conn.brexPort, null);  
			}else {
				msg=conn.ReadString(); 
				cmd.setResult(status, msg.toString()+" "+conn.brexPrimary+"/"+conn.brexPort, null);  
			}
  
		} catch (Exception e) {
			e.printStackTrace();
		}
		return cmd;
	}
	public BaseCommand CMD_CREATEFILE(CFAPI5J conn, HashMap param, BaseCommand cmd) {
		boolean status=false;
		String msg="";
		try {
			conn.WriteString( (String) param.get("TARGET_FILE") );
			conn.WriteByteFile(   (byte[])param.get("FILE_SOURCE") );
			conn.WriteString( (String) param.get("FILE_CHECKSUM") );
			conn.WriteString( (String) param.get("CHECKSUM_TYPE") );
			conn.WriteString( ""+(long) param.get("FILE_LAST_MODIFIED") );
			conn.WriteString( (String) param.get("FILE_PERMISSION") );
			conn.MBRS_Run();
			status = conn.ReadInt()==0 ? true:false ;
			if( status ) {
				msg=conn.ReadString();
				cmd.setResult(status, msg, null);  
			}else {
//				msg=conn.ReadString();
//				cmd.setResult(status, msg.toString()+" "+conn.brexPrimary+"/"+conn.brexPort, null);  
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return cmd;
	}

	public BaseCommand CMD_DOSEARCH_ONLY_FILE(CFAPI5J conn, HashMap param, BaseCommand cmd) {
//		inHash.put("TARGET_IP", "172.16.15.15");
////		inHash.put("TARGET_IP", "127.0.0.1");
//		inHash.put("TARGET_PORT", "30502");
//		inHash.put("CONNECT_TYPE", "A");
//		inHash.put("MACHINE_TYPE", "S");
//		
//		String toFile = "/home/cf/temp/a001";
//		inHash.put("TARGET_FILE", toFile); //원격지 파일 경로
		boolean status=false;
		String msg=""; 
		try {
//			inHash.put("TARGET_PATH", "D:/50_INSTALL/SampleBiz/real/java"); //가져올 파일 수집 최상위 경로
//			inHash.put("TARGET_REGEXP", "/compressionFilters/Compression[-*_*.*A-Za-z0-9]*.java"); //정규식 1번 예시 - Test로 시작하는 .html파일

			
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
		}
		return resultCmd;
	}
}
