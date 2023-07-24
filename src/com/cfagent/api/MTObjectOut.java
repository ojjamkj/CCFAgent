package com.cfagent.api;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.OutputStreamWriter;

 
public class MTObjectOut {
	private int m_allocLength;

	private ByteArrayOutputStream byteOut;

	private DataOutputStream m_OutBuffer;
	private String errMsg = "";
	private int temp;
 
	public MTObjectOut() {
		m_allocLength =4096;
		byteOut = new ByteArrayOutputStream();
		m_OutBuffer = new DataOutputStream(byteOut);

	}
 
	public MTObjectOut(int length) {
		m_allocLength = length;
		byteOut = new ByteArrayOutputStream();
		m_OutBuffer = new DataOutputStream(byteOut);

	}
 
	public void Clear() {
		if( byteOut !=null){
			byteOut.reset();
		}

	}

	protected void finalize() throws Throwable {
		super.finalize();
	}
 
	public int getLength() {
		return byteOut.size();
	}
 
	public ByteArrayOutputStream getMBuffer() {
		return byteOut;
	}

	public byte[] getM_buffer() {
		return byteOut.toByteArray();
	}

	public boolean WriteByte(int b) {
		errMsg = "";
		try {
			temp = b;
		} catch (Exception e) {
			temp = (byte) 0;
		}
		if (byteOut.size() + 1 > m_allocLength){
			if( m_allocLength<1000000){
				m_allocLength=byteOut.size() +1;
			}else{
				return false;
			}
		}

		try {
			m_OutBuffer.writeByte((byte) temp);
		} catch (Exception e) {
			errMsg = e.toString();
			return false;
		}
		return true;
	}

	public void AddInt(int lVal) {
		WriteInt(1);
		WriteInt(lVal);
	}

	public boolean WriteInt(int lVal) {
		WriteString("" + lVal);
		return true;
	}
 
	public void AddFraction(double dblVal) {
		WriteInt(1);
		WriteString("" + dblVal);
	}

	public void AddString(String strVal) {
		WriteInt(1);
		WriteString(strVal);
	}
	public boolean WriteString(String strVal) {
		errMsg = "";
		if (strVal == null) {
			strVal = "";
		}
		int leng = 0;
		byte[] bytBytes = { 0 };
		try {
			leng = strVal.getBytes().length;
			bytBytes = strVal.getBytes();
			byte b1 = (byte) (leng / 256);
			byte b2 = (byte) (leng % 256);
			m_OutBuffer.writeByte(b1);
			m_OutBuffer.writeByte(b2);
			m_OutBuffer.write(bytBytes);

			return true;
		} catch (Exception e) {
			errMsg = e.toString();
			return false;
		}
 
	}
	public boolean WriteByteFile( byte bytBytes[]) {
		WriteString(""+bytBytes.length);
		errMsg = "";
		if (bytBytes == null) {
			bytBytes = "".getBytes();
		}
		int leng = 0;
		try {
			m_OutBuffer.write(bytBytes);
			return true;
		} catch (Exception e) {
			errMsg = e.toString();
			return false;
		}
 
	}

	public boolean WriteByte( byte bytBytes[]) {
		errMsg = "";
		if (bytBytes == null) {
			bytBytes = "".getBytes();
		}
		int leng = 0;
		try {
			leng = bytBytes.length;
			byte b1 = (byte) (leng / 256);
			byte b2 = (byte) (leng % 256);
			m_OutBuffer.writeByte(b1);
			m_OutBuffer.writeByte(b2);
			m_OutBuffer.write(bytBytes);

			return true;
		} catch (Exception e) {
			errMsg = e.toString();
			return false;
		}
 
	}
	public String errMsg() {
		return errMsg;
	}
}
