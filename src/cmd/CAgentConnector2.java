package cmd;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.HashMap;

import com.cfagent.api.CFAPI5J;
import com.gtone.cf.daemon.cmd.BaseCommand;
import com.gtone.cf.rt.connect.impl.AbstractConnector;
import com.gtone.cf.rt.file.FileDeployCommand;
import com.gtone.cf.rt.file.FileModel;
import com.gtone.cf.util.ICFConstants;

import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

 
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

		BaseCommand resultCmd = new FileDeployCommand(); 
		try {
			
			conn.WriteString( (String) param.get("TARGET_PATH") );
			conn.WriteString( (String) param.get("TARGET_REGEXP") );
			conn.WriteString( (param.get("INCLUDE_SUB_DIR") != null && "Y".equals((String) param.get("INCLUDE_SUB_DIR"))) ? "Y" : "N"   );
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

					JSONArray viewDirList = JSONArray.fromObject(jsonstring);
					int size  = viewDirList.size();
					System.out.println("jsonstring size = " + size);
					ArrayList resultList = new ArrayList();
					for(int i=0; i<size; i++)
					{
						
						FileModel fileModel = setFileModel(conn, (JSONObject)viewDirList.get(i), remoteTargetRootPath, false);
						if(fileModel != null) {
							resultList.add(fileModel);
							System.out.println(fileModel.getPath());
							fileModel.setFileSource(conn.ReadFileByte());
						}
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
	
	public BaseCommand CMD_VIEWDIR(CFAPI5J conn, HashMap param, BaseCommand cmd) {
		BaseCommand resultCmd = new FileDeployCommand();
		try {
			conn.WriteString( (String) param.get("TARGET_PATH") );
			conn.WriteString( (param.get("TARGET_REGEXP") != null) ? (String) param.get("TARGET_REGEXP") : "" );
			conn.WriteString( (param.get("INCLUDE_SUB_DIR") != null && "Y".equals((String) param.get("INCLUDE_SUB_DIR"))) ? "Y" : "N"   );
			conn.WriteString( (param.get("DEFAULT_GET_ROWS") != null) ?((String) param.get("DEFAULT_GET_ROWS")) : "-1" );
			
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
//					System.out.println(jsonstring);
					JSONArray viewDirList = JSONArray.fromObject(jsonstring);
					int size  = viewDirList.size();
					System.out.println("jsonstring size = " + size);
					ArrayList resultList = new ArrayList();
					for(int i=0; i<size; i++)
					{
						FileModel fileModel = setFileModel(conn, (JSONObject)viewDirList.get(i), remoteTargetRootPath, false);
						if(fileModel != null) resultList.add(fileModel);
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
	
	public BaseCommand CMD_BUILD(CFAPI5J conn, HashMap param, BaseCommand cmd) {
		BaseCommand resultCmd = new FileDeployCommand();
		try {
			String command =  (String) param.get("BUILD_LOC");
			if(param.containsKey("BUILD_PARAM")) {
				String[] commandP = (String[])param.get("BUILD_PARAM");
				for(String c : commandP)
				{
					command += " " + c;
				}
			}
			conn.WriteString( command );
			conn.WriteString( (String) param.get("BUILD_FILE_TYPE") );
			
			conn.MBRS_Run();
			
			boolean status = conn.ReadInt()==0 ? true:false ;
			
			if( status ) {
				String result = conn.ReadString();
				String returnValue =conn.ReadString();
				String message =conn.ReadLongString();
				
				HashMap resultMap = new HashMap();
				
				if(new Boolean(result).booleanValue()) {
					resultMap.put("BUILD_RESULT_RETURN", returnValue);
					resultMap.put("BUILD_RESULT_CONSOLE", message);
					
					resultCmd.setResult(true, null, resultMap);
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
	
	public BaseCommand CMD_DOSEARCH_ONLY_DIR(CFAPI5J conn, HashMap param, BaseCommand cmd) {

		BaseCommand resultCmd = new FileDeployCommand(); 
		try {
			
			conn.WriteString( (String) param.get("TARGET_PATH") );
			conn.MBRS_Run();
			
			boolean status = conn.ReadInt()==0 ? true:false ;
			
			if( status ) {
				String result = conn.ReadString();
				String message =conn.ReadString();
				
				if(new Boolean(result).booleanValue()) {
					String remoteTargetRootPath = (String)param.get("TARGET_PATH");
					
					String jsonstring = conn.ReadLongString();
					if(jsonstring== null || "".equals(jsonstring)) {
						throw new Exception("received dir info is null");
					}

					JSONArray viewDirList = JSONArray.fromObject(jsonstring);
					int size  = viewDirList.size();
					System.out.println("jsonstring size = " + size);
					ArrayList resultList = new ArrayList();
					for(int i=0; i<size; i++)
					{
						JSONObject dirObj = (JSONObject)viewDirList.get(i);
						if(dirObj.containsKey("path")) {
							resultList.add(dirObj.get("path"));
						}
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
	
	public BaseCommand CMD_SCANDIR_TO_FILE(CFAPI5J conn, HashMap param, BaseCommand cmd) {

		BaseCommand resultCmd = new FileDeployCommand(); 
		try {
			
			conn.WriteString( (String) param.get("TARGET_PATH") );
//			conn.WriteString( (String) param.get("INCLUDE_CHECKSUM") );
//			conn.WriteString( (String) param.get("INCLUDE_CHECKSUM_TYPE") );
			 
//			inHash.put("INCLUDE_FILTER", new ArrayList()); //옵션
//			inHash.put("IGNORE_FILTER", new ArrayList()); //옵션		
			 
			conn.MBRS_Run();
			
			boolean status = conn.ReadInt()==0 ? true:false ;
			
			if( status ) {
				String result = conn.ReadString();
				String message = conn.ReadString();
				
				if(new Boolean(result).booleanValue()) {
					FileModel fileModel = new FileModel();
					fileModel.setFileSource(conn.ReadFileByte()); 
					resultCmd.setResult(true, null, fileModel);
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
		if(!fileObj.containsKey("filename")) return null;
		
		if(fileObj.containsKey("filename")) {
			file.setFilename(fileObj.getString("filename"));
			System.out.println(fileObj.getString("filename"));
		}
		if(fileObj.containsKey("isdir")) {
			file.setIsDirectory(fileObj.getInt("isdir")==1? true: false);
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
		case BaseCommand.CMD_BUILD:
			resultCmd = CMD_BUILD(conn,param,cmd);
			break;
		case BaseCommand.CMD_DOSEARCH_ONLY_DIR:
			resultCmd = CMD_DOSEARCH_ONLY_DIR(conn,param,cmd);
			break;
		case BaseCommand.CMD_SCANDIR_TO_FILE:
			resultCmd = CMD_SCANDIR_TO_FILE(conn,param,cmd);
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
