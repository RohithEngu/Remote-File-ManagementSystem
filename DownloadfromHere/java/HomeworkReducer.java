
/*
 * HomeworkReducer.java
 *************************************************************
 * Author : Rohith Engu										*
 * SUID : 678923180											*
 * Email : roengu@syr.edu									*
 * Application : Cloud Computing Homework Assignment 2		*
 * 															*
 ************************************************************
 */

package org.smaple;


import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;



// import org.apache.commons.logging.Log;
// import org.apache.commons.logging.LogFactory;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.io.Text;

/**
 *
 * @author mac
 */
public class HomeworkReducer extends Reducer<Text,Text,Text,Text> {
	// The Karmasphere Studio Workflow Log displays logging from Apache Commons Logging, for example:
	// private static final Log LOG = LogFactory.getLog("org.smaple.HomeworkReducer");
	
	@Override
	protected void reduce(Text key, Iterable<Text> values, Context context)
			throws IOException, InterruptedException {		
		String val = "";
		for(Text v : values){
			val+=v+",";
		}
		val = val.substring(0, val.length() - 1);
		context.write(key, new Text(val));
	}
}

