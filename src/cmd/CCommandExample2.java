package cmd;
/*
TEST server ==> 172.16.15.15  putty : cf / gtone1004

1.기동   cd /home/cf/Bin
 cfagent ./BRMCONF.ini  ( BRMCONF1.ini 는 현재 의미 없음.. 추후 변경 예정 ) 
  구동시 기본 포트 : 35400(실제 사용포트 ), 35401(추후 제거대상 포트 : 소스 로직 미 분리로 아직 살아 있음 . ) 
            ==> 해당 포트 정보는 MRLODRUL.cpp 에 존재 ( 해당 소스도 추후 삭제 예정 ).
2.컴파일
cd /home/cf/Dev/Build
컴파일 명령어 :   engine.scr
결과 파일 :   /home/cf/Dev//Bin/LINUX//cfagent
서버 관련 주요 소스 
 2.1 /home/cf/Dev/Src/Admin/ 
    BRMiner.cpp   ->MainProcess()함수에 기능에 따라 CFAPI.API03_PING .. 함수 호출
    BRStart.cpp
2.2 /home/cf/Dev/CFAPI
    CFAPI.cpp     --> 기능별로 함수 분리되어 있음..
2.3 /home/cf/Dev/Include
    CFAPI.h
    MTUtil.h
2.4 /home/cf/Dev/Src/Util
    MTUtil.cpp   -> CFAPI, MTUtil 에 cfagent 프로그램에서 필요한 함수가 양쪽으로 나뉘어짐 .. 하나로 합쳐야 함..

*/
      
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.zip.GZIPInputStream;

import com.gtone.cf.daemon.cmd.BaseCommand;
import com.gtone.cf.rt.connect.IConnector;
import com.gtone.cf.rt.connect.impl.CAgentConnector;
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
			HashMap inHash = new HashMap();
			inHash.put("TARGET_IP", "172.16.15.15");
			inHash.put("TARGET_PORT", "30503");
//			inHash.put("TARGET_IP", "127.0.0.1");
//			inHash.put("TARGET_PORT", "30502");
			inHash.put("CONNECT_TYPE", "B");
			inHash.put("MACHINE_TYPE", "S");
			long start = System.currentTimeMillis();
			CCommandExample2 obj = new CCommandExample2();
			//1.CMD_AGENT_PING
 			obj.ping(inHash); 			// 일부처리
			//2.CMD_CREATEFILE
//			obj.createFile(inHash);		// 일부처리0
//			obj.createMultiFile(inHash, new File("D:/50_INSTALL/SampleBiz/dev/MediaHub_CCI"));		// 일부처리0
			//3.CMD_VIEWFILE
//			obj.viewFile(inHash); 		// 일부처리
			//4.CMD_BUILD
//			obj.build(inHash);
			//5.CMD_DELETEFILE
//			obj.deleteFile();  		// 일부처리
			//6.CMD_DOSEARCH_ONLY_FILE
//			obj.searchOnlyFile(inHash);
			//6.CMD_DOSEARCH_ONLY_FILE
//			obj.searchOnlyFileCollector(inHash);
			//7.CMD_DOSEARCH_ONLY_DIR
//			obj.searchOnlyDir(inHash);
			//8.CMD_VIEWDIR
//			obj.viewDir(inHash);
			//9.CMD_SCANDIR_TO_FILE		
//			obj.scanToFile(inHash);
			long end = System.currentTimeMillis();
			
			System.out.println(end-start);
		}catch(Exception e) {
			e.printStackTrace();
		}
	}
	/// 여기서 부터 커맨드 하나씩 붙이면 됩니다.
	public void ping(HashMap inHash) throws Exception
	{
		


		BaseCommand cmd = new FileDeployCommand( BaseCommand.CMD_AGENT_PING );
		cmd.setCommandType(BaseCommand.CMD_TYPE_DEPLOY);
		cmd.setValue(ICFConstants.HASHMAP, inHash);
		cmd.setMultiple(false);

		//여기서 
		BaseCommand resultCmd = remoteCmdRun(inHash, inHash, cmd);
		if( !resultCmd.isSuccess() )
			throw new Exception( resultCmd.getErrorMessage() );
		else {
			
			System.out.println("API-ping: resultCmd.getMessage():"+resultCmd.getResultData());
//			System.out.println(resultCmd.getResultData());
		}
	}
	public void createMultiFile(HashMap inHash, File rootFile) throws Exception
	{
		String rootDir = "D:/50_INSTALL/SampleBiz/dev/MediaHub_CCI";
//		File rootFile = new File(rootDir);
		File[] files = rootFile.listFiles();
		
		String targetRoot = "/home/cf/tofile";
		
		for(File f: files)
		{
			if(f.isFile()) {
				String fromFile = f.getAbsolutePath();
				String toFile = f.getAbsolutePath().replaceAll("\\\\", "/").replaceFirst(rootDir, "");
				toFile = targetRoot + toFile;
				createFile(fromFile, toFile, inHash);
			}else {
				createMultiFile(inHash, f);
			}
		}
	}
	public void createFile(String fromFile, String toFile, HashMap inHash) throws Exception
	{
				
		
//		String fromFile = "D:/temp/sinhyup/bucasu4084.c";
//		String toFile = "/home/cf/tofile/bucasu4084.c";
//		String toFile = "/home/cf/tofile/AAA.XML";
//		String fromFile ="D:\\temp\\tofile\\AAA.XML";
		byte[] source = FileManager.viewFile(fromFile);
		inHash.put("TARGET_FILE", toFile); //원격지 파일 경로
		inHash.put("FILE_SOURCE", FileManager.viewFile(fromFile) ); //
		inHash.put("TARGET_PATH", "/home/cf/tofile"); 
		inHash.put("FILE_CHECKSUM", CheckSumUtil.getCheckSum(source, "2"));
		inHash.put("CHECKSUM_TYPE", "2"); //sha256		
		inHash.put("FILE_LAST_MODIFIED", new File(fromFile).lastModified() );
		inHash.put("FILE_PERMISSION", "744");


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

	public void viewFile(HashMap inHash) throws Exception
	{
//		String toFile = "/home/cf/Dev/Src/Admin/BREXDiag.cpp";
		String toFile = "/home/cf/tofile/src/main/java/com/ccieurope/webservices/adinsertion/AdInsertionDataRequestHardErr.java";
//		byte[] source = FileManager.viewFile(toFile);
		inHash.put("TARGET_FILE", toFile); //원격지 파일 경로
		inHash.put("TARGET_PATH", "/home/cf/Dev/Bin/LINUX"); 
		
 

		BaseCommand cmd = new FileDeployCommand( BaseCommand.CMD_VIEWFILE );
		cmd.setCommandType(BaseCommand.CMD_TYPE_DEPLOY);
		cmd.setValue(ICFConstants.HASHMAP, inHash);
		cmd.setMultiple(false);
		
		//여기서 
		BaseCommand resultCmd = remoteCmdRun(inHash, inHash, cmd);
		if( !resultCmd.isSuccess() )
			throw new Exception( resultCmd.getErrorMessage() );
		else {
			FileModel fileModel = (FileModel)resultCmd.getResultData();
			System.out.println(resultCmd.getResultData());
			System.out.println(fileModel.getFileSource());
			
			FileManager.createFile(fileModel.getFileSource(), "D:/temp/AdInsertionDataRequestHardErr.java");
		}
	}
	
	public void build(HashMap inHash) throws Exception
	{
		inHash.put("BUILD_LOC", "/home/cf/test/changeflow.sh"); //원격지 파일 경로
//		inHash.put("BUILD_FILE_TYPE", "0"); 
		inHash.put("BUILD_PARAM", new String[] {"param1", "param2"});
//		inHash.put("LOG_TYPE", "CONSOLE");
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
	
	public void deleteFile(HashMap inHash) throws Exception
	{
		
		
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
	
	public void searchOnlyFile(HashMap inHash) throws Exception
	{
		//NotFoundDirException | FileNotFoundException 에 대한 에러 처리 반드시 필요...원격에서 오류가 발생한 경우 반드시 서버로 해당 오류를 보내주어야함.
		
		//배포시 파라미터 케이스2 (정규식 사용,)
		inHash.put("TARGET_PATH", "/home/cf/tofile"); //가져올 파일 수집 최상위 경로
//		inHash.put("TARGET_REGEXP", "[-*_*.*A-Za-z0-9]*.java"); //정규식 1번 예시 - Test로 시작하는 .html파일
//		inHash.put("TARGET_REGEXP", "[-*_*.*A-Za-z0-9]*.java"); //정규식 1번 예시 - Test로 시작하는 .html파일
//		inHash.put("TARGET_REGEXP", ".*"); //정규식 2번 예시 - 하위 디렉토리 모든 파일 
//		inHash.put("TARGET_REGEXP", "/compressionFilters/Compression($.*)?.class"); //정규식 3번 예시 - Compression 클래스, inner 클래스 포함
		inHash.put("TARGET_REGEXP", "(.)*\\.java"); //정규식 3번 예시 - Compression 클래스, inner 클래스 포함
		inHash.put("INCLUDE_SUB", "Y"); 
		
		
		
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
			for(FileModel fileModel : files)
			{
				FileManager.createFile(fileModel.getFileSource(), fileModel.getPath().replaceAll("/home/cf/tofile","D:/temp/ttt"));
			}	
		}
	}
	
	
	public void searchOnlyFileCollector(HashMap inHash) throws Exception
	{
		ArrayList includeFilter = new ArrayList();
//		includeFilter.add(FileManager.getMatchedRegExp("**.java"));
		ArrayList ignoreFilter = new ArrayList();
//		ignoreFilter.add(FileManager.getMatchedRegExp("**.bak"));
		inHash.put("TARGET_PATH", "/home/cf/tofile/GapMiner"); //가져올 파일 수집 최상위 경로
		inHash.put("INC_FILTER", includeFilter); 
		inHash.put("EXC_FILTER", ignoreFilter); 
		
		int fileCount = 0;
		int currentSize = 0;
		ArrayList fileList = new ArrayList();
		do
		{	
			inHash.put("START_ROW", ""+fileCount); //시작위치
			
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
				for(FileModel fileModel : files)
				{
					if(fileModel.getErrorMsg() != null && !"".equals(fileModel.getErrorMsg() )) {
						System.out.println(fileModel.getPath() + ": " + fileModel.getErrorMsg());
						continue;
					}
					
					FileManager.createFile(fileModel.getFileSource(), fileModel.getPath().replaceAll("/home/cf/tofile","D:/temp/ttt"));
					
					if(fileList.contains(fileModel.getPath())) {
						System.out.println("dup: " + fileModel.getPath());
					}else {
						fileList.add(fileModel.getPath());
					}
				}
				currentSize = files.size();
				fileCount += currentSize;
				
			}
		}while(currentSize > 0);
	}
	
	
	public void searchOnlyDir(HashMap inHash) throws Exception
	{
		
		inHash.put("TARGET_PATH", "/home/cf/tofile"); 
		
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
	
	
	public void viewDir(HashMap inHash) throws Exception
	{
		inHash.put("TARGET_PATH", "/home/cf/tofile"); 
		inHash.put("INCLUDE_SUB_DIR", "Y"); //하위 디렉토리 포함 여부
//		inHash.put("TARGET_REGEXP", "(.)*(.)*"); //검색 정규식, 옵션
		inHash.put("TARGET_REGEXP", "(.)*\\.png"); //검색 정규식, 옵션
		inHash.put("DEFAULT_GET_ROWS", "10"); //옵션
		
		
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
//				System.out.println(model.getPath());
//				System.out.println(model.getSize());
//				System.out.println(model.getCanRead());
//				System.out.println(model.isCanWrite());
//				System.out.println(model.getFilename());
//				System.out.println(model.getFileSource());
//				System.out.println(model.isDirectory());
//				System.out.println(model.getLastModifiedDate());
//				System.out.println(model.getLength());
//				System.out.println(model.getParent());
//				System.out.println(model.getRelPath());
//				System.out.println(model.getRootPath());
//				System.out.println(model.getType());
//				System.out.println("");
			}	
		}
	}
	
	public void scanToFile(HashMap inHash) throws Exception
	{
		ArrayList includeFilter = new ArrayList();
//		includeFilter.add(FileManager.getMatchedRegExp("**.vm"));
		ArrayList ignoreFilter = new ArrayList();
//		ignoreFilter.add(FileManager.getMatchedRegExp("**.bak"));
		
		inHash.put("TARGET_PATH", "/home/cf/tofile"); 
		inHash.put("INCLUDE_FILTER", includeFilter); //옵션
		inHash.put("IGNORE_FILTER", ignoreFilter); //옵션		
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
			String savedFile = "d:/Temp/scanResultFile.gz";
			String unzipFile = "d:/Temp/unzipScanResultFile.txt";
			FileManager.createFile(scanFileSource, savedFile);
			unzipGzFile(savedFile, unzipFile);
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
			Class connClass = Class.forName("cmd.CAgentConnector2");
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
			CAgentConnector ca2 = new CAgentConnector();
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
	
	public static void unzipGzFile(String gzFilePath, String outputFilePath) throws IOException {
	    try (FileInputStream fis = new FileInputStream(gzFilePath);
	         GZIPInputStream gzipInputStream = new GZIPInputStream(fis);
	         FileOutputStream fos = new FileOutputStream(outputFilePath)) {
	        
	        byte[] buffer = new byte[1024];
	        int len;
	        while ((len = gzipInputStream.read(buffer)) > 0) {
	            fos.write(buffer, 0, len);
	        }
	    }
	}
	
//	protected void unzip(String zipFile, File unzipFileName) {
//
//        FileInputStream fileInputStream = null;	
//        FileOutputStream fileOutputStream = null;
//        ZipInputStream zipInputStream = null;
//        try {
//            fileInputStream = new FileInputStream(zipFile);
//            zipInputStream = new ZipInputStream(fileInputStream);
//            ZipEntry zipEntry = null;
//            
//            byte[] buf = new byte[1024];
//            int length = 0;
//            while ((zipEntry = zipInputStream.getNextEntry()) != null) {
//                fileOutputStream = new FileOutputStream(unzipFileName);                
//                
//    			while ((length = zipInputStream.read(buf)) != -1) {
//                    fileOutputStream.write(buf, 0, length);
//                }
//
//                zipInputStream.closeEntry();
//                fileOutputStream.flush();
//                fileOutputStream.close();
//            }
//            zipInputStream.close();
//        } catch (IOException e) {
//            // Exception Handling
//        } finally {
//            if(zipInputStream != null) try {zipInputStream.closeEntry(); }catch(Exception e) {}
//            if(fileOutputStream != null) try {fileOutputStream.flush(); }catch(Exception e) {}
//            if(fileOutputStream != null) try {zipInputStream.close(); }catch(Exception e) {}
//            if(zipInputStream != null) try {zipInputStream.close(); }catch(Exception e) {}
//        }
//    }




}

