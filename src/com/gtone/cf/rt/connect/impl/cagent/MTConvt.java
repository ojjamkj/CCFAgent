package com.gtone.cf.rt.connect.impl.cagent;

public class MTConvt {
	public MTConvt() {
		super();
	} 
	public static int ReorderBytesInt(int v) {
		int v0, v1, v2, v3;

		v0 = (v << 24) & 0xFF000000;
		v1 = (v << 8)  & 0x00FF0000;
		v2 = (v >> 8)  & 0x0000FF00;
		v3 = (v >> 24) & 0x000000FF;
		return v0 + v1 + v2 + v3;
	}
	public static long ReorderBytesLong(long v) {
		long v0, v1, v2, v3, v4, v5, v6, v7;
		v0 = ((v >> 56) & 0x00000000000000FF);
		v1 = ((v >> 48) & 0x00000000000000FF) << 8;
		v2 = ((v >> 40) & 0x00000000000000FF) << 16;
		v3 = ((v >> 32) & 0x00000000000000FF) << 24;
		v4 = ((v >> 24) & 0x00000000000000FF) << 32;
		v5 = ((v >> 16) & 0x00000000000000FF) << 40;
		v6 = ((v >> 8)  & 0x00000000000000FF) << 48;
		v7 = ((v)       & 0x00000000000000FF) << 56;
		return v0 | v1 | v2 | v3 | v4 | v5 | v6 | v7;

	}

}
