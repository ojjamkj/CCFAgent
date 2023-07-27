package com.gtone.cf.rt.connect.impl.cagent;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;

 
public class MTObject {
	private int m_allocLength;
	private ByteArrayOutputStream byteOut;
	private DataInputStream m_InBuffer=null;

	private ByteArrayInputStream byteIn;
	private DataOutputStream m_OutBuffer;
	private int temp;

	/**
	 * MTObject constructor comment.
	 */
	public MTObject() {
		m_allocLength = 4096;
		byteOut = new ByteArrayOutputStream();
		m_OutBuffer = new DataOutputStream(byteOut);
	}

	/**
	 * MTObject constructor comment.
	 */
	public MTObject(long reOrder ) {
		m_allocLength = 4096;
		byteOut = new ByteArrayOutputStream();
		m_OutBuffer = new DataOutputStream(byteOut);

	}

	/**
	 * MTObject constructor comment.
	 */
	public MTObject(long reOrder, int length) {
		m_allocLength = length;
		byteOut = new ByteArrayOutputStream();
		m_OutBuffer = new DataOutputStream(byteOut);
	}

 
	public void Clear() {
		byteOut.reset();
		byteIn.reset();

	}

	protected void finalize() throws Throwable {
		// Insert code to finalize the receiver here.
		// This implementation simply forwards the message to super. You may
		// replace or supplement this.
		super.finalize();
	}

 
	public int getLength() {
		return byteOut.size();
	}

 
	public byte[] getM_buffer() {
		return byteOut.toByteArray();
	}

 
	public void setM_buffer(byte[] inData) {
		byteIn = new ByteArrayInputStream(inData);
		m_InBuffer = new DataInputStream(byteIn);
	}

	public boolean WriteByte(int b) {

		if (byteOut.size() + 1 > m_allocLength)
			return false;
		try {
			temp = b;
		} catch (Exception e) {
			temp = 0;
		}
		try {
			m_OutBuffer.writeByte((byte) temp);
		} catch (Exception e) {
			System.err.println(e.toString());
			return false;
		}
		return true;
	}
 
	public boolean WriteString(String strVal) {

		if (strVal == null) {
			strVal = "";
		}
		int leng = strVal.getBytes().length;

		if (byteOut.size() + leng > m_allocLength)
			return false;

		try {
			m_OutBuffer.writeBytes(strVal);
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}

		return true;
	}
}
