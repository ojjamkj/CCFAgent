package com.gtone.cf.rt.connect.impl;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.HashMap;

import com.gtone.cf.daemon.cmd.BaseCommand;
import com.gtone.cf.rt.connect.impl.cagent.CFAPI5J;
import com.gtone.cf.rt.file.FileDeployCommand;
import com.gtone.cf.rt.file.FileModel;
import com.gtone.cf.util.ICFConstants;

import jspeed.base.util.StringHelper;
import net.sf.json.JSONArray;
import net.sf.json.JSONNull;
import net.sf.json.JSONObject;

 
public class CAgentConnector extends AbstractConnector {
	SimpleDateFormat sdf = new SimpleDateFormat(ICFConstants.DATE_PATTERN);
	
	public BaseCommand CMD_AGENT_PING(CFAPI5J conn, HashMap param, BaseCommand cmd) {
		
		BaseCommand resultCmd = new FileDeployCommand();
		
		try {
			conn.MBRS_Run();
			
			boolean status = conn.ReadInt()==0 ? true:false ;
			if( status ) {
				String result = conn.ReadString();
				String message =conn.ReadString();
				
				if(new Boolean(result).booleanValue()) {
					String version =conn.ReadString();
					String startTime =conn.ReadString();
					String installDir =conn.ReadString();
					HashMap versionInfoMap = new HashMap();
					versionInfoMap.put("AGENT_BUNDLE_VERSION", version);
					versionInfoMap.put("AGENT_START_TIME", startTime);
					versionInfoMap.put("AGENT_INSTALL_DIR", installDir);
					
					
					// set result
					HashMap dataHash = new HashMap();
					dataHash.put(ICFConstants.CMD_RESULT, "I AM ALIVE");
					dataHash.put("AGENT_META_INFO", versionInfoMap);

					resultCmd.setResult(true, null, dataHash);
					resultCmd.setValue(ICFConstants.CMD_RESULT,"true");
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
					
					System.out.println(jsonstring);
					
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
	
	
	public FileModel CMD_VIEWFILE(HashMap param, String targetFile, String targetPath) throws Exception{
		
		try {
			CFAPI5J conn = new CFAPI5J();
			String ip = (String)param.get(ICFConstants.TARGET_IP);
			int port = Integer.parseInt( (String)param.get(ICFConstants.TARGET_PORT));
			
			conn.brexPrimary=ip;
			conn.brexPort=port;
			conn.Initialize("" + BaseCommand.CMD_VIEWFILE);
				
			conn.WriteString( targetFile );
			conn.MBRS_Run();
			
			boolean status = conn.ReadInt()==0 ? true:false ;
			
			if( status ) {
				String result = conn.ReadString();
				String message =conn.ReadString();
				
				if(new Boolean(result).booleanValue()) {
					String remoteTargetRootPath = targetPath;
					 
					String jsonstring = conn.ReadString();
					if(jsonstring== null || "".equals(jsonstring)) {
						throw new Exception("received file info is null");
					}
					
//					System.out.println(jsonstring);
					
					FileModel file = setFileModel(conn, JSONObject.fromObject(jsonstring), remoteTargetRootPath, true);
					
					return file;
				}				
			}
		} catch (Exception e) {
			throw(e);
		}
		return null;
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
						if(viewDirList.get(i)==null || viewDirList.get(i) instanceof JSONNull) continue;
						String filePath = (String)viewDirList.get(i);
						param.put("TARGET_FILE", filePath);
						
						
						FileModel fileModel = this.CMD_VIEWFILE(param, filePath, remoteTargetRootPath);
						
						if(fileModel != null) {
							int fileLength = (int)fileModel.getLength();
							System.out.println(fileModel.getPath());							
							resultList.add(fileModel);
						}else {
							throw new Exception("fail to get file=" + filePath);
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
	
	public BaseCommand CMD_DOSEARCH_ONLY_FILE_COLLECT(CFAPI5J conn, HashMap param, BaseCommand cmd) {

		BaseCommand resultCmd = new FileDeployCommand(); 
		try {
			
			conn.WriteString( (String) param.get("TARGET_PATH") );
			conn.WriteString( (String) param.get("START_ROW") );
			
			ArrayList<String> list = (ArrayList)param.get("INCLUDE_FILTER");
			conn.WriteString( ""+list.size());
			for(String m: list) {
				conn.WriteString(m);
			}
			
			list = (ArrayList)param.get("IGNORE_FILTER");
			conn.WriteString( ""+list.size());
			for(String m: list) {
				conn.WriteString(m);
			}
			
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
					System.out.println("jsonstring = " + jsonstring);
					
					ArrayList resultList = new ArrayList();					
					for(int i=0; i<size; i++)
					{
						if(viewDirList.get(i)==null || viewDirList.get(i) instanceof JSONNull) {
							System.out.println("TARGET_FILE0 : " + viewDirList.get(i));
							continue;
						}
						
						String filePath = (String)viewDirList.get(i);
						//param.put("TARGET_FILE1", filePath);
						
						
						FileModel fileModel = this.CMD_VIEWFILE(param, filePath, remoteTargetRootPath);
						
						if(fileModel != null) {
							int fileLength = (int)fileModel.getLength();
//							System.out.println(fileModel.getPath());							
							resultList.add(fileModel);
						}else {
							fileModel = new FileModel();
							fileModel.setPath(filePath);
							fileModel.setErrorMsg("can't read file");
							resultList.add(fileModel);
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
						if(viewDirList.get(i)==null || viewDirList.get(i) instanceof JSONNull) continue;
						
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
						String dirPath = (String)viewDirList.get(i);
						if(!dirPath.equals("")) {
							resultList.add(dirPath);
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
			
			ArrayList<String> list = (ArrayList)param.get("INCLUDE_FILTER");
			conn.WriteString( ""+list.size());
			for(String m: list) {
				conn.WriteString(m);
			}
			
			list = (ArrayList)param.get("IGNORE_FILTER");
			conn.WriteString( ""+list.size());
			for(String m: list) {
				conn.WriteString(m);
			}

			
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
//			System.out.println(fileObj.getString("filename"));
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
		if(fileObj.containsKey("errmsg")) file.setErrorMsg(fileObj.getString("errmsg"));
		if(includeSource && StringHelper.evl(file.getErrorMsg(), null)==null) file.setFileSource(conn.ReadFileByte());
		
		return file;
	}
	public BaseCommand remoteCmdRun(HashMap globalMap, HashMap param, BaseCommand cmd) throws Exception{
		CFAPI5J conn = null;
		try {
			conn = new CFAPI5J();
			String ip = (String)param.get(ICFConstants.TARGET_IP);
			int port = Integer.parseInt( (String)param.get(ICFConstants.TARGET_PORT));
			BaseCommand resultCmd = null;
			conn.brexPrimary=ip;
			conn.brexPort=port;
			
			switch(cmd.getCommand()) {
			case BaseCommand.CMD_AGENT_PING:
				conn.Initialize("" + cmd.getCommand()); 
				resultCmd = CMD_AGENT_PING(conn,param,cmd);
				break;
			case BaseCommand.CMD_CREATEFILE:
				conn.Initialize("" + cmd.getCommand()); 
				resultCmd = CMD_CREATEFILE(conn,param,cmd);
				break;
			case BaseCommand.CMD_VIEWFILE:
				conn.Initialize("" + cmd.getCommand()); 
				resultCmd = CMD_VIEWFILE(conn,param,cmd);
				break;			
			case BaseCommand.CMD_DELETEFILE:
				conn.Initialize("" + cmd.getCommand()); 
				resultCmd = CMD_DELTEFILE(conn,param,cmd);
				break;
			case BaseCommand.CMD_DOSEARCH_ONLY_FILE:
				if(param.containsKey("INCLUDE_FILTER")) {
					conn.Initialize("41");
					resultCmd = CMD_DOSEARCH_ONLY_FILE_COLLECT(conn,param,cmd);
				}else {
					conn.Initialize("" + cmd.getCommand());
					resultCmd = CMD_DOSEARCH_ONLY_FILE(conn,param,cmd);
				}
				
				break;
			case BaseCommand.CMD_VIEWDIR:
				conn.Initialize("" + cmd.getCommand());
				resultCmd = CMD_VIEWDIR(conn,param,cmd);
				break;
			case BaseCommand.CMD_BUILD:
				conn.Initialize("" + cmd.getCommand());
				resultCmd = CMD_BUILD(conn,param,cmd);
				break;
			case BaseCommand.CMD_DOSEARCH_ONLY_DIR:
				conn.Initialize("" + cmd.getCommand());
				resultCmd = CMD_DOSEARCH_ONLY_DIR(conn,param,cmd);
				break;
			case BaseCommand.CMD_SCANDIR_TO_FILE:
				conn.Initialize("" + cmd.getCommand());
				resultCmd = CMD_SCANDIR_TO_FILE(conn,param,cmd);
				break;	
				
			}
			return resultCmd;
		}catch(Exception e) {
			throw(e);
		}finally {
			
		}
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
