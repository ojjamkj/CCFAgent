package com.gtone.cf.rt.connect.impl.cagent;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;

public class MTObjectIn {
	private DataInputStream m_InBuffer;
	private ByteArrayInputStream byteIn;
	private String errMsg = "";

	public MTObjectIn(byte[] inData) {
		byteIn = new ByteArrayInputStream(inData);
		m_InBuffer = new DataInputStream(byteIn);
	} 
	public void Clear() {
		byteIn.reset();
	}
	public void close() {
		if(byteIn!= null) {
			try {
				byteIn.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		if(m_InBuffer!= null) {
			try {
				m_InBuffer.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
//	protected void finalize() throws Throwable {
//		super.finalize();
//	}
	public byte ReadByte() {
		errMsg = "";
		byte b = 0;
		try {
			b = m_InBuffer.readByte();
		} catch (Exception e) {
			errMsg = e.toString();
		}
		return b;
	}
	public double ReadDouble() throws Exception {
		String temp=ReadString();
		Double dValue = new Double( temp.trim().equals("") ? ""+0 : temp.trim());
		return dValue.doubleValue();
	}

	public int ReadInt() {
		int dValue = 0;
		try {
			dValue = new Integer(ReadString()).intValue();
		} catch (Exception e) {
			// TODO: handle exception
		}

		return dValue;

	}

	public double ReadFraction() throws Exception {

		return ReadDouble();

	}

	public String ReadString() throws Exception {
		try {
			int leng;
			byte[] byteData = null;
			int tx;
			String ret="";
			leng = (int) m_InBuffer.readByte() & 0xff;
			tx = (int) m_InBuffer.readByte() & 0xff;
			leng = (leng * 256) + tx;
			byteData = new byte[leng];
			//System.out.println("Total ReadString leng("+leng+")");
			if( leng ==26990) {
				int tx2 = (int) m_InBuffer.readByte() & 0xff;
				//				System.out.println("ReadString tx leng("+ ((tx*256)+tx2)+") tx2("+tx2+")");
			}
			m_InBuffer.read(byteData, 0, leng);
			ret= (new String(byteData));
			return ret;
		} catch (Exception e) {
			throw(e);
		}

	}
	public String ReadLongString() throws Exception {
		try {
			int leng;
			byte[] byteData = null;
			int tx;
			String ret="";
			leng = (int) m_InBuffer.readByte() & 0xff;
			tx = (int) m_InBuffer.readByte() & 0xff;
			leng = (leng * 256) + tx;
//			System.out.println(" leng : "+leng);         
			byteData = new byte[leng];
			m_InBuffer.read(byteData, 0, leng);
			ret= (new String(byteData));
//			System.out.println(" ret : "+ret.toString());         

			leng= Integer.parseInt(ret.trim());
			byteData = new byte[leng];
//			System.out.println(" leng2 : "+ leng );         
			m_InBuffer.read(byteData, 0, leng);
			ret= (new String(byteData));
			return ret;
		} catch (Exception e) {
			throw(e);
		}

	}
//	public  String ReadFile(String path,String fileName) {
//		try {
//			int leng;
//			byte[] byteData = null;
//			int tx; 
//			String ret="";
//			leng=Integer.parseInt(ReadString() );
//
//			byteData = new byte[leng];
//			long start , end;
//			start =System.currentTimeMillis();
//			//System.out.println("Total ReadString leng("+leng+")");
//			int size=m_InBuffer.read(byteData, 0, leng);
//			end =System.currentTimeMillis();
//			//System.out.println("Read ("+(end-start)+")");
//			File file=new File(path+fileName);
//			FileOutputStream fos = new FileOutputStream(file);
//			fos.write(byteData); 
//			fos.flush();
//			fos.close();
//			start =System.currentTimeMillis();
//			//System.out.println("write ("+(start-end)+")");
//			//			ret= (new String(byteData));
//			return " ";
//		} catch (IOException ie) {
//			ie.printStackTrace();
//			return " ";
//		} catch (Exception e) {
//			e.printStackTrace();
//			return " ";
//		}
//
//	}
	public String ReadFile() throws Exception {
		try {
			int leng;
			byte[] byteData = null;
			int tx;
			String ret="";
			leng = (int) m_InBuffer.readByte() & 0xff;
			tx = (int) m_InBuffer.readByte() & 0xff;
			leng = leng * 256 + tx;
			byteData = new byte[leng];

			m_InBuffer.read(byteData, 0, leng);
			ret= (new String(byteData));
			return ret;
		} catch (Exception e) {
			throw(e);
		}

	}
	public  byte[] ReadFileByte() throws Exception {
		try {
			int leng;
			byte[] byteData = null;
			int tx; 
			String ret="";
			leng=Integer.parseInt(ReadString() );


			//			int leng;
			//			byte[] byteData = null;
			//			int tx;
			//			String ret="";
			//			leng = (int) m_InBuffer.readByte() & 0xff;
			//			tx = (int) m_InBuffer.readByte() & 0xff;
			//			leng = (leng * 256) + tx;
			//			byteData = new byte[leng];	

			byteData = new byte[leng];



			int size=m_InBuffer.read(byteData, 0, leng);

			return byteData;

		} catch (IOException ie) {
			throw(ie);

		} catch (Exception e) {
			throw(e);

		}
	}
	public  byte[] ReadFileByte(int leng) throws Exception {
		try {

			byte[] byteData = new byte[leng];

			int size=m_InBuffer.read(byteData, 0, leng);

			return byteData;

		} catch (IOException ie) {
			throw(ie);

		} catch (Exception e) {
			throw(e);
		}
	}	
	public String ReadString(String charsetName) throws Exception {
		try {
			int leng;
			byte[] byteData = null;
			int tx;

			leng = (int) m_InBuffer.readByte() & 0xff;
			tx = (int) m_InBuffer.readByte() & 0xff;
			leng = leng * 256 + tx;
			byteData = new byte[leng];

			m_InBuffer.read(byteData, 0, leng);
			if(charsetName!=null && !charsetName.trim().equals("")){
				return new String(byteData, charsetName);
			}else{
				return (new String(byteData));
			}
		} catch (Exception e) {
			throw(e);
		}

	}

	public void setBuffer(byte[] inData) {
		// byteIn.reset();

		byteIn = new ByteArrayInputStream(inData);
		m_InBuffer = new DataInputStream(byteIn);
	}

	
}
