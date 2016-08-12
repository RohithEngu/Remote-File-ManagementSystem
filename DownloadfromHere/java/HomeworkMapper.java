/*
 * HomeworkMapper.java
 ************************************************************
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
import java.util.Map;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.io.Text;

/**
 *
 * @author mac
 */
public class HomeworkMapper extends Mapper<Text,Text,Text,Text> {
	private static HashMap<String,HashSet<String>> initialMap = new HashMap<String, HashSet<String>>();
	@Override
	protected void map(Text key, Text value, Context context)
			throws IOException, InterruptedException {
		if(!key.toString().equalsIgnoreCase(value.toString()))
		{
			HashSet<String> l = initialMap.get(key.toString());
			if(l==null)
				initialMap.put(key.toString(),l=new HashSet<String>());
			l.add(value.toString());
		}
	}

	/* (non-Javadoc)
	 * @see org.apache.hadoop.mapreduce.Mapper#cleanup(org.apache.hadoop.mapreduce.Mapper.Context)
	 */
	@Override
	protected void cleanup(Mapper<Text, Text,Text,Text>.Context context)
			throws IOException, InterruptedException {
		ArrayList<String> urls = new ArrayList<String>();
		HashMap<ArrayList<String>,HashSet<String>> map = new HashMap<ArrayList<String>, HashSet<String>>();
		for(Map.Entry<String, HashSet<String>> entry : initialMap.entrySet()){
			for(String st : entry.getValue())
			{
				urls.add(st);
			}
			ArrayList<String> UrlKeys;

			for(int i=0;i<urls.size();i++){
				for(int j=i+1;j<urls.size();j++)
				{
					UrlKeys = new ArrayList<String>();
					UrlKeys.add(urls.get(i));
					UrlKeys.add(urls.get(j));
					HashSet<String> l = map.get(UrlKeys);
					if(l==null)
						map.put(UrlKeys,l=new HashSet<String>());
					l.add(entry.getKey());
				}
			}

			urls.clear();
		}
		for(Map.Entry<ArrayList<String>, HashSet<String>> entry : map.entrySet()){
			boolean flag1 = false,flag2=false;
			for(Map.Entry<String, HashSet<String>> entry1 : initialMap.entrySet()){
				if(entry1.getKey().equalsIgnoreCase(entry.getKey().get(0))){
					if(entry1.getValue().size()==1){
						HashSet<String> chk = entry1.getValue();
						if(chk.contains(entry.getKey().get(1))){
							flag1=true;
							break;
						}
					}
				}
			}
			for(Map.Entry<String, HashSet<String>> entry1 : initialMap.entrySet()){
				if(entry1.getKey().equalsIgnoreCase(entry.getKey().get(1))){
					if(entry1.getValue().size()==1){
						HashSet<String> chk = entry1.getValue();
						if(chk.contains(entry.getKey().get(0))){
							flag2 = true;
							break;
						}
					}
				}
			}
			if(flag1 && flag2){
				String keyOutput1 = "",keyOutput2="",valueOutput="";
				for(String v : entry.getValue()){
					valueOutput+= v +",";
				}
				valueOutput = valueOutput.substring(0, valueOutput.length() - 1);
				for(int i=0;i<entry.getKey().size();++i){
					keyOutput1+= entry.getKey().get(i) +",";
				}
				keyOutput1 = keyOutput1.substring(0, keyOutput1.length() - 1);
				context.write(new Text(keyOutput1),new Text(valueOutput));
				for(int i=entry.getKey().size()-1;i>=0;i--){
					keyOutput2+= entry.getKey().get(i) +",";
				}
				keyOutput2 = keyOutput2.substring(0, keyOutput2.length() - 1);
				context.write(new Text(keyOutput2),new Text(valueOutput));
			}
			else{
				String keyOutput = "",valueOutput="";
				for(String k : entry.getKey()){
					keyOutput+= k +",";
				}
				keyOutput = keyOutput.substring(0, keyOutput.length() - 1);
				for(String v : entry.getValue()){
					valueOutput+= v +",";
				}
				valueOutput = valueOutput.substring(0, valueOutput.length() - 1);
				context.write(new Text(keyOutput),new Text(valueOutput));
			}				
		}
		super.cleanup(context);
	}
}



