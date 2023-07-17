package cmd;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Base64;
import java.util.HashMap;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import com.gtone.cf.daemon.cmd.BaseCommand;
import com.gtone.cf.rt.connect.IConnector;
import com.gtone.cf.rt.file.FileDeployCommand;
import com.gtone.cf.rt.file.FileManager;
import com.gtone.cf.rt.file.FileModel;
import com.gtone.cf.util.CheckSumUtil;
import com.gtone.cf.util.ICFConstants;

import jspeed.base.util.StringHelper;

public class CCommandExample2 {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		try {
			CCommandExample2 obj = new CCommandExample2();
			//1.CMD_AGENT_PING
// 			obj.ping(); 			// 일부처리
			//2.CMD_CREATEFILE
//			obj.createFile();		// 일부처리
			//3.CMD_VIEWFILE
//			obj.viewFile(); 		// 일부처리
			//4.CMD_BUILD
//			obj.build();
			//5.CMD_DELETEFILE
//			obj.deleteFile();  		// 일부처리
			//6.CMD_DOSEARCH_ONLY_FILE
			obj.searchOnlyFile();
			//7.CMD_DOSEARCH_ONLY_DIR
//			obj.searchOnlyDir();
			//8.CMD_VIEWDIR
//			obj.viewDir();
			//9.CMD_SCANDIR_TO_FILE		
//			obj.scanToFile();

		}catch(Exception e) {
			e.printStackTrace();
		}
	}
	/// 여기서 부터 커맨드 하나씩 붙이면 됩니다.
	public void ping() throws Exception
	{
		HashMap inHash = new HashMap();
		inHash.put("TARGET_IP", "172.16.15.15");
		inHash.put("TARGET_PORT", "34000");
//		inHash.put("TARGET_IP", "127.0.0.1");
//		inHash.put("TARGET_PORT", "30502");
		inHash.put("CONNECT_TYPE", "A");
		inHash.put("MACHINE_TYPE", "S");


		BaseCommand cmd = new FileDeployCommand( BaseCommand.CMD_AGENT_PING );
		cmd.setCommandType(BaseCommand.CMD_TYPE_DEPLOY);
		cmd.setValue(ICFConstants.HASHMAP, inHash);
		cmd.setMultiple(false);

		//여기서 
		BaseCommand resultCmd = remoteCmdRun(inHash, inHash, cmd);
		if( !resultCmd.isSuccess() )
			throw new Exception( resultCmd.getErrorMessage() );
		else {
			
			System.out.println("API-ping: resultCmd.getMessage():"+resultCmd.getMessage());
//			System.out.println(resultCmd.getResultData());
		}
	}
	
	public void createFile() throws Exception
	{
		HashMap inHash = new HashMap();
		inHash.put("TARGET_IP", "172.16.15.15");
//		inHash.put("TARGET_IP", "127.0.0.1");
		inHash.put("TARGET_PORT", "34000");
//		inHash.put("TARGET_IP", "127.0.0.1");
//		inHash.put("TARGET_PORT", "30502");
		inHash.put("CONNECT_TYPE", "A");
		inHash.put("MACHINE_TYPE", "S");
		
		String toFile = "/home/cf/tofile/AAA.XML";
//		String fromFile = "D:/temp/FROMFILE/AAA.xml";
//		String toFile = "/home/cf//tofile/AAA.XML";
		String fromFile ="D:\\temp\\tofile\\AAA.XML";
		byte[] source = FileManager.viewFile(fromFile);
		inHash.put("TARGET_FILE", toFile); //원격지 파일 경로
//		inHash.put("FILE_SOURCE", Base64.getEncoder().encodeToString(FileManager.viewFile(fromFile) )); //
		inHash.put("FILE_SOURCE", FileManager.viewFile(fromFile) ); //
		inHash.put("TARGET_PATH", "D:/temp"); 
		inHash.put("FILE_CHECKSUM", CheckSumUtil.getCheckSum(source, "2"));
		inHash.put("CHECKSUM_TYPE", "2"); //sha256		
		inHash.put("FILE_LAST_MODIFIED", new File(fromFile).lastModified() );
		inHash.put("FILE_PERMISSION", "755");


		BaseCommand cmd = new FileDeployCommand( BaseCommand.CMD_CREATEFILE );
		cmd.setCommandType(BaseCommand.CMD_TYPE_DEPLOY);
		cmd.setValue(ICFConstants.HASHMAP, inHash);
		cmd.setMultiple(false);
		
		//여기서 
		BaseCommand resultCmd = remoteCmdRun(inHash, inHash, cmd);
		if( !resultCmd.isSuccess() )
			throw new Exception( resultCmd.getErrorMessage() );
		else {
			System.out.println("API-createFile: resultCmd.getMessage():"+resultCmd.getMessage());
		}
		
	}

	public void viewFile() throws Exception
	{
		HashMap inHash = new HashMap();
		inHash.put("TARGET_IP", "172.16.15.15");
//		inHash.put("TARGET_IP", "127.0.0.1");
		inHash.put("TARGET_PORT", "34000");
		inHash.put("CONNECT_TYPE", "A");
		inHash.put("MACHINE_TYPE", "S");
		
//		String toFile = "/home/cf/Dev/Src/Admin/BREXDiag.cpp";
		String toFile = "/home/cf/Dev//Bin/LINUX/cfagent";
//		byte[] source = FileManager.viewFile(toFile);
		inHash.put("TARGET_FILE", toFile); //원격지 파일 경로
		inHash.put("TARGET_PATH", "D:/temp"); 
		
 

		BaseCommand cmd = new FileDeployCommand( BaseCommand.CMD_VIEWFILE );
		cmd.setCommandType(BaseCommand.CMD_TYPE_DEPLOY);
		cmd.setValue(ICFConstants.HASHMAP, inHash);
		cmd.setMultiple(false);
		
		//여기서 
		BaseCommand resultCmd = remoteCmdRun(inHash, inHash, cmd);
		if( !resultCmd.isSuccess() )
			throw new Exception( resultCmd.getErrorMessage() );
		else {
//			FileModel fileModel = (FileModel)resultCmd.getResultData();
//			System.out.println(resultCmd.getResultData());
//			System.out.println(fileModel.getFileSource());
		}
	}
	
	public void build() throws Exception
	{
		HashMap inHash = new HashMap();
		inHash.put("TARGET_IP", "127.0.0.1");
		inHash.put("TARGET_PORT", "30502");
		inHash.put("CONNECT_TYPE", "A");
		inHash.put("MACHINE_TYPE", "S");
		

		inHash.put("BUILD_LOC", "D:/50_INSTALL/SampleBiz/dev/eachCall.bat"); //원격지 파일 경로
		inHash.put("BUILD_FILE_TYPE", "0"); 
		inHash.put("BUILD_PARAM", new String[] {"param1"});
		inHash.put("LOG_TYPE", "CONSOLE");
		inHash.put("BUILD_OUTPUT", "");

		BaseCommand cmd = new FileDeployCommand(  BaseCommand.CMD_BUILD );
		cmd.setCommandType(BaseCommand.CMD_TYPE_BUILD);
		cmd.setValue(ICFConstants.HASHMAP, inHash);
		cmd.setMultiple(false);
		
		//여기서 
		BaseCommand resultCmd = remoteCmdRun(inHash, inHash, cmd);
		if( !resultCmd.isSuccess() )
			throw new Exception( resultCmd.getErrorMessage() );
		else {
			System.out.println(StringHelper.evl(resultCmd.getResultData(), ""));
		}
		
	}
	
	public void deleteFile() throws Exception
	{
		HashMap inHash = new HashMap();
		inHash.put("TARGET_IP", "172.16.15.15");  
//		inHash.put("TARGET_IP", "127.0.0.1");
		inHash.put("TARGET_PORT", "34000");
		inHash.put("CONNECT_TYPE", "A");
		inHash.put("MACHINE_TYPE", "S");
		
		String toFile = "/home/cf/temp/a001";
		inHash.put("TARGET_FILE", toFile); //원격지 파일 경로
		

		BaseCommand cmd = new FileDeployCommand( BaseCommand.CMD_DELETEFILE );
		cmd.setCommandType(BaseCommand.CMD_TYPE_DEPLOY);
		cmd.setValue(ICFConstants.HASHMAP, inHash);
		cmd.setMultiple(false);
		
		//여기서 
		BaseCommand resultCmd = remoteCmdRun(inHash, inHash, cmd);
		if( !resultCmd.isSuccess() )
			System.out.println("API-deleteFile: resultCmd.getMessage():"+resultCmd.getMessage());
//			throw new Exception( resultCmd.getErrorMessage() );
		else {
			System.out.println("API-deleteFile: resultCmd.getMessage():"+resultCmd.getMessage());
		}
	}
	
	public void searchOnlyFile() throws Exception
	{
		//NotFoundDirException | FileNotFoundException 에 대한 에러 처리 반드시 필요...원격에서 오류가 발생한 경우 반드시 서버로 해당 오류를 보내주어야함.
		
		HashMap inHash = new HashMap();
		inHash.put("TARGET_IP", "172.16.15.15");
		inHash.put("TARGET_PORT", "34000");
		inHash.put("CONNECT_TYPE", "A");
		inHash.put("MACHINE_TYPE", "S");
		
		//배포시 파라미터 케이스2 (정규식 사용,)
		inHash.put("TARGET_PATH", "/home/cf/temp/java"); //가져올 파일 수집 최상위 경로
		inHash.put("TARGET_REGEXP", "[-*_*.*A-Za-z0-9]*.java"); //정규식 1번 예시 - Test로 시작하는 .html파일
//		inHash.put("TARGET_REGEXP", ".*"); //정규식 2번 예시 - 하위 디렉토리 모든 파일 
//		inHash.put("TARGET_REGEXP", "/compressionFilters/Compression($.*)?.class"); //정규식 3번 예시 - Compression 클래스, inner 클래스 포함
		
		
		BaseCommand cmd = new FileDeployCommand( BaseCommand.CMD_DOSEARCH_ONLY_FILE );
		cmd.setCommandType(BaseCommand.CMD_TYPE_DEPLOY);
		cmd.setValue(ICFConstants.HASHMAP, inHash);
		cmd.setMultiple(false);
		
		//여기서 
		BaseCommand resultCmd = remoteCmdRun(inHash, inHash, cmd);
		if( !resultCmd.isSuccess() )
			throw new Exception( resultCmd.getErrorMessage() );
		else {
			ArrayList<FileModel> files = (ArrayList<FileModel>)resultCmd.getResultData();
			for(FileModel model : files)
			{
				System.out.println(model.getPath());
				System.out.println(model.getSize());
				System.out.println(model.getCanRead());
				System.out.println(model.isCanWrite());
				System.out.println(model.getFilename());
				System.out.println(model.getFileSource());
				System.out.println(model.isDirectory());
				System.out.println(model.getLastModifiedDate());
				System.out.println(model.getLength());
				System.out.println(model.getParent());
				System.out.println(model.getRelPath());
				System.out.println(model.getRootPath());
				System.out.println(model.getType());
			}	
		}
	}
	
	
	public void searchOnlyDir() throws Exception
	{
		HashMap inHash = new HashMap();
		inHash.put("TARGET_IP", "127.0.0.1");
		inHash.put("TARGET_PORT", "30502");
		inHash.put("CONNECT_TYPE", "A");
		inHash.put("MACHINE_TYPE", "S");
		
		
		inHash.put("TARGET_PATH", "D:/50_INSTALL/SampleBiz/real"); 
		
		
		
		BaseCommand cmd = new FileDeployCommand( BaseCommand.CMD_DOSEARCH_ONLY_DIR );
		cmd.setCommandType(BaseCommand.CMD_TYPE_DEPLOY);
		cmd.setValue(ICFConstants.HASHMAP, inHash);
		cmd.setMultiple(false);
		
		//여기서 
		BaseCommand resultCmd = remoteCmdRun(inHash, inHash, cmd);
		if( !resultCmd.isSuccess() )
			throw new Exception( resultCmd.getErrorMessage() );
		else {
			ArrayList<String> files = (ArrayList<String>)resultCmd.getResultData();
			for(String dir : files)
			{
				System.out.println(dir);
				
			}	
		}
	}
	
	
	public void viewDir() throws Exception
	{
		HashMap inHash = new HashMap();
		inHash.put("TARGET_IP", "127.0.0.1");
		inHash.put("TARGET_PORT", "30502");
		inHash.put("CONNECT_TYPE", "A");
		inHash.put("MACHINE_TYPE", "S");
		
		ArrayList includeFilter = new ArrayList();
		includeFilter.add("/*.java");
		ArrayList ignoreFilter = new ArrayList();
		ignoreFilter.add("**.bak");
		
		inHash.put("TARGET_PATH", "D:/50_INSTALL/SampleBiz/real"); 
		inHash.put("INCLUDE_SUB_DIR", "Y"); //하위 디렉토리 포함 여부
		inHash.put("TARGET_REGEXP", ""); //검색 정규식, 옵션
		inHash.put("INCLUDE_FILTER", new ArrayList()); //옵션
		inHash.put("IGNORE_FILTER", new ArrayList()); //옵션
		inHash.put("START_ROW", "0"); //옵션
		inHash.put("DEFAULT_GET_ROWS", "500"); //옵션
		
		
		BaseCommand cmd = new FileDeployCommand( BaseCommand.CMD_VIEWDIR );
		cmd.setCommandType(BaseCommand.CMD_TYPE_DEPLOY);
		cmd.setValue(ICFConstants.HASHMAP, inHash);
		cmd.setMultiple(false);
		
		//여기서 
		BaseCommand resultCmd = remoteCmdRun(inHash, inHash, cmd);
		if( !resultCmd.isSuccess() )
			throw new Exception( resultCmd.getErrorMessage() );
		else {
			ArrayList<FileModel> files = (ArrayList<FileModel>)resultCmd.getResultData();
			for(FileModel model : files)
			{
				System.out.println(model.getPath());
				System.out.println(model.getSize());
				System.out.println(model.getCanRead());
				System.out.println(model.isCanWrite());
				System.out.println(model.getFilename());
				System.out.println(model.getFileSource());
				System.out.println(model.isDirectory());
				System.out.println(model.getLastModifiedDate());
				System.out.println(model.getLength());
				System.out.println(model.getParent());
				System.out.println(model.getRelPath());
				System.out.println(model.getRootPath());
				System.out.println(model.getType());
				System.out.println("");
			}	
		}
	}
	
	public void scanToFile() throws Exception
	{
		HashMap inHash = new HashMap();
		inHash.put("TARGET_IP", "127.0.0.1");
		inHash.put("TARGET_PORT", "30502");
		inHash.put("CONNECT_TYPE", "A");
		inHash.put("MACHINE_TYPE", "S");
		
		ArrayList includeFilter = new ArrayList();
		includeFilter.add("/*.java");
		ArrayList ignoreFilter = new ArrayList();
		ignoreFilter.add("**.bak");
		
		inHash.put("TARGET_PATH", "D:/50_INSTALL/SampleBiz/real"); 
		inHash.put("INCLUDE_FILTER", new ArrayList()); //옵션
		inHash.put("IGNORE_FILTER", new ArrayList()); //옵션		
		inHash.put("INCLUDE_CHECKSUM", "Y"); //체크썸 포함 여부
		inHash.put("INCLUDE_CHECKSUM_TYPE", "CRC"); //검색 정규식, 옵션
		
		
		BaseCommand cmd = new FileDeployCommand( BaseCommand.CMD_SCANDIR_TO_FILE );
		cmd.setCommandType(BaseCommand.CMD_TYPE_DEPLOY);
		cmd.setValue(ICFConstants.HASHMAP, inHash);
		cmd.setMultiple(false);
		
		//여기서 
		BaseCommand resultCmd = remoteCmdRun(inHash, inHash, cmd);
		if( !resultCmd.isSuccess() )
			throw new Exception( resultCmd.getErrorMessage() );
		else {
			FileModel model = (FileModel)resultCmd.getResultData();
			
			byte[] scanFileSource = model.getFileSource();
			String savedFile = "d:/Temp/scanResultFile.txt";
			String unzipFile = "d:/Temp/unzipScanResultFile.txt";
			FileManager.createFile(scanFileSource, "d:/Temp/scanResultFile.txt");
			unzip(savedFile, new File(unzipFile));
			//아래는 리턴값 샘플, 뉴라인키가 각 데이터의 구분자, 파일은 utf-8로 생성되어야함.
			/** 
			 * 
			 * {NAME : "build.bat",ISDIR : false,MDATE : "20201014145445",PATH : "D:/50_INSTALL/SampleBiz/real/build.bat",RELPATH : "build.bat",ROOTPATH : "D:/50_INSTALL/SampleBiz/real",READ : true,WRITE : true,SIZE : 24,CHECKSUM : "f38249f"}
{NAME : "build1.bat",ISDIR : false,MDATE : "20201014145445",PATH : "D:/50_INSTALL/SampleBiz/real/build1.bat",RELPATH : "build1.bat",ROOTPATH : "D:/50_INSTALL/SampleBiz/real",READ : true,WRITE : true,SIZE : 24,CHECKSUM : "f38249f"}
{NAME : "eachCall.bat",ISDIR : false,MDATE : "20200123131312",PATH : "D:/50_INSTALL/SampleBiz/real/eachCall.bat",RELPATH : "eachCall.bat",ROOTPATH : "D:/50_INSTALL/SampleBiz/real",READ : true,WRITE : true,SIZE : 28,CHECKSUM : "d10d75a8"}

			 */
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//공통 (건들지 마세요)
	public IConnector getConnector(String machineType, String connectType){

		try{
			Class connClass = Class.forName("cmd.CAgentConnector");
			IConnector con =  (IConnector)connClass.newInstance();
			con.setMachineType(machineType);
			con.setConnectType(connectType);
			return con;
		}catch(Exception e){
			return null;
		}
	}
	//공통 (건들지 마세요)
	protected Object makeConnection(HashMap globalMap,HashMap param,IConnector conn, boolean isSave  ) throws Exception {


		Object client = conn.open(  param );

		param.put(client.getClass().getName() , client);

		return client;
	}
	//공통 (건들지 마세요)
	public BaseCommand remoteCmdRun(HashMap globalMap, HashMap param, BaseCommand cmd) throws Exception
	{
		String connectType = (String)param.get(ICFConstants.CONNECT_TYPE);
		String machineType = (String)param.get(ICFConstants.MACHINE_TYPE);
		String ip = (String)param.get(ICFConstants.TARGET_IP);
		String port = (String)param.get(ICFConstants.TARGET_PORT);



		BaseCommand runCmd = cmd;


//		IConnector connector = getConnector(machineType, connectType);
//
//		connector.setIp(ip);
//		connector.setPort(port);

		Object conn = null;
		try
		{
//			conn = makeConnection(globalMap, param, connector,runCmd.isMultiple()  );
//			BaseCommand result = connector.connect(runCmd);
			CAgentConnector2 ca2 = new CAgentConnector2();
			BaseCommand result =ca2.remoteCmdRun(globalMap, param, runCmd);
			return result;
		}
		catch(Exception e)
		{
			throw new Exception(ip + ":" + port + "==>" + e.getMessage());
		}
		finally
		{
//			if(conn != null && !runCmd.isMultiple())
//			{
//				connector.close(conn);
//				if( param.containsKey(Socket.class.getName())) {
//					param.remove(Socket.class.getName());
//				}
//			}
		}

	} 
	
	protected void unzip(String zipFile, File unzipFileName) {

        FileInputStream fileInputStream = null;	
        FileOutputStream fileOutputStream = null;
        ZipInputStream zipInputStream = null;
        try {
            fileInputStream = new FileInputStream(zipFile);
            zipInputStream = new ZipInputStream(fileInputStream);
            ZipEntry zipEntry = null;
            
            byte[] buf = new byte[1024];
            int length = 0;
            while ((zipEntry = zipInputStream.getNextEntry()) != null) {
                fileOutputStream = new FileOutputStream(unzipFileName);                
                
    			while ((length = zipInputStream.read(buf)) != -1) {
                    fileOutputStream.write(buf, 0, length);
                }

                zipInputStream.closeEntry();
                fileOutputStream.flush();
                fileOutputStream.close();
            }
            zipInputStream.close();
        } catch (IOException e) {
            // Exception Handling
        } finally {
            if(zipInputStream != null) try {zipInputStream.closeEntry(); }catch(Exception e) {}
            if(fileOutputStream != null) try {fileOutputStream.flush(); }catch(Exception e) {}
            if(fileOutputStream != null) try {zipInputStream.close(); }catch(Exception e) {}
            if(zipInputStream != null) try {zipInputStream.close(); }catch(Exception e) {}
        }
    }




}

