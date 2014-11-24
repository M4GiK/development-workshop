<?
		include "../lib/File.php";
		include "../lib/Graph.php";
		include "../admin/config.php";
		 "../lib/phplot/phplot.php";
		function buildBarsGraph($username,$date, $names,$values,$title)
		{
			$output = OUTPATH."/$username-$date.txt";
			$file = new File($output, "result");
			$parsed = $file->parse();
			//print_r($parsed);
			
			$info['data'] = array();
		
		
			for($i = 0; $i < count($values); $i++)
			{
				$data = array($names[$i],str_replace("%","",$parsed[$values[$i]]));
				array_push($info['data'],$data);
			}
		
			$info['title_Y'] = "time(ms)";
			$info['showYLabel'] = true;
			$info['title'] = $title;
			$info['data_type'] = "text-data";
			$info['graph_type'] = "bars";
			$info['is_legend'] = false;
			$graph = new Graph($info,350,200);
			//$graph = new test($info);
			$graph->draw();
	}
	function buildNumberHostGraph($username,$date)
	{
		
			
			$output = OUTPATH."/$username-$date.txt";
			$file = new File($output, "result");
			$parsed = $file->parse();
			//print_r($parsed);
			
			$info['data'] = array();
			$data = array("min",$parsed["Min.Number Hosts"],$parsed["Min.Number Free Hosts"]);
			array_push($info['data'],$data);
			
			$data = array("max",$parsed["Max.Number Hosts"],$parsed["Max.Number Free Hosts"]);
			array_push($info['data'],$data);
			
			$data = array("avg",$parsed["Avg.Number Hosts"],$parsed["Avg.Number Free Hosts"]);
			array_push($info['data'],$data);
			
			$data = array("sd",$parsed["Sd. Number Hosts"],$parsed["Sd. Number Free Hosts"]);
			array_push($info['data'],$data);
		
			$info['title_Y'] = "number hosts(integer)";
			$info['YTickIncrement'] = 2;
			$info['showYLabel'] = true;
			$info['title'] = "Number Hosts";
			$info['data_type'] = "text-data";
			$info['graph_type'] = "bars";
			$info['is_legend'] = true;
			$info['legend'] = array("total","free");
			$graph = new Graph($info,350,200);
			$graph->draw();
	}
	function buildRunsTimeGraph($username,$date)
	{
		
			
			$output = OUTPATH."/$username-$date.txt";
			$file = new File($output, "result");
			$parsed = $file->parse();
		
			$info['data'] = array();
			$data = array("min",$parsed["Minimum makespan"],$parsed["Minimum flowtime"]);
			array_push($info['data'],$data);
			
			$data = array("max",$parsed["Maximum makespan"],$parsed["Maximum flowtime"]);
			array_push($info['data'],$data);
			
			$data = array("avg",$parsed["Average makespan"],$parsed["Average flowtime"]);
			array_push($info['data'],$data);
			
			$data = array("sd",$parsed["Sd. makespan"],$parsed["Sd. flowtime"]);
			array_push($info['data'],$data);
			
			$info['title'] = "Time";
			$info['data_type'] = "text-data";
			$info['graph_type'] = "bars";
			$info['is_legend'] = true;
			$info['legend'] = array("Makespan","Flowtime");
			$graph = new Graph($info,350,200);
			$graph->draw();
	}
	function buildPieGraph($username,$date)
	{
		$output = OUTPATH."/$username-$date.txt";
		$file = new File($output, "result");
		$parsed = $file->parse();
		$info['data'] = array(
								array("Total Idle Time",str_replace("%","",$parsed["Total Idle Time"])),
								array("Total Busy Time",str_replace("%","",$parsed["Total Busy Time"])),
								array("Total Lost Time",str_replace("%","",$parsed["Total Lost Time"])));
		$info['title'] = "Time";
		$info['data_type'] = "text-data-single";
		$info['graph_type'] = "pie";
		$info['is_legend'] = true;
		$info['legend'] = array();
		
		foreach ($info["data"] as $row)
  			array_push($info["legend"],implode(' % : ', $row));
		
		$graph = new Graph($info,350,200);
		$graph->draw();
	}
	
	function buildTimeLineTrace($username, $date, $trace)
	{
		$output = OUTPATH."/$username-trace-$date.txt";
		$file = new File($output, "trace");
		$traces = $file->parse();
		$parsed = $traces[$trace];
		
		
		$time = "0.0";
		$evn_enter = false;
		$evn_start = false;
		$evn_finish = false;
		$evn_new_host = false;
		$evn_stop_host = false;
		$evn_schedule = false;
		
		$info['data'] = array();
		$i = 0;
		while($i < count($parsed))
		{
			$time_act = ereg_replace("^0+", "", $parsed[$i]["time"]);
			while($time_act == $time && $i<count($parsed))
			{
				switch($parsed[$i]["event"])
				{
					case "EVN_NEW_HOST":
											$evn_new_host = true;
											break;
					
					case "EVN_ENTER":
										$evn_enter = true;
										break;
										
					case "EVN_SCHEDULE":
											$evn_schedule = true;
											break;
					case "EVN_START":
										$evn_start = true;
										break;
					case "EVN_FINISH":
										$evn_finish = true;
										break;
					
					case "EVN_STOP_HOST":	
											$evn_stop_host = true;
											break; 
				}
				
				$time_act = ereg_replace("^0+", "", $parsed[$i]["time"]);
				$i++;
			}
			
			if($evn_enter == true)
			{
				$data = array('',$time,0,'','','','','');
				array_push($info['data'],$data);
			}
			
			if($evn_start == true)
			{
				$data = array('',$time,'',1,'','','','');
				array_push($info['data'],$data);
			}
			
			
			if($evn_finish == true)
			{
				$data = array('',$time,'','',2,'','','');
				array_push($info['data'],$data);
			}
			
			if($evn_new_host == true)
			{
				$data = array('',$time,'','','',3,'','');
				array_push($info['data'],$data);
			}
			
			if($evn_stop_host == true)
			{
				$data = array('',$time,'','','','',4,'');
				array_push($info['data'],$data);
			}
		
			if($evn_schedule == true)
			{
				$data = array('',$time,'','','','','',5);
				array_push($info['data'],$data);
			}
			
			$evn_enter = false;
			$evn_start = false;
			$evn_finish = false;
			$evn_new_host = false;
			$evn_stop_host = false;
			$evn_schedule = false;
			$time = $time_act;
			
			
		}		
		
		$info['showXLabel'] = true;
		$info['showYLabel'] = false;
		$info['XTickIncrement'] = 5000;
		$info['YTickIncrement'] = 1;
		
		$info['title'] = "time line trace";
		$info['data_type'] = "data-data";
		$info['graph_type'] = "points";
		$info['is_legend'] = true;
		$info['legend'] = array("enter","start","finish","new host","stop host","schedule");
		
		$graph = new Graph($info,800,200);
		$graph->draw();
		//return "<img scr=\"".$graph->draw()."\"/>";
	}
	
	
	function buildHostTrace($username, $date, $trace)
	{
		$output = OUTPATH."/$username-trace-$date.txt";
		$file = new File($output, "trace");
		$traces = $file->parse();
		$parsed = $traces[$trace];
		
		$time = "0.0";
		$evn_enter = false;
		$evn_start = false;
		$evn_finish = false;
		$evn_new_host = false;
		$evn_stop_host = false;
		$evn_schedule = false;
		
		$info['data'] = array();
		$i = 0;
		$hosts = 0;
		while($i < count($parsed))
		{
			$time_act = ereg_replace("^0+", "", $parsed[$i]["time"]);
			while($time_act == $time && $i<count($parsed))
			{
				switch($parsed[$i]["event"])
				{
					case "EVN_NEW_HOST":
											$hosts++;
											$evn_new_host = true;
											break;
					
					
					case "EVN_STOP_HOST":	
											$hosts--;
											$evn_stop_host = true;
											break; 
				}
				
				$time_act = ereg_replace("^0+", "", $parsed[$i]["time"]);
				$i++;
			}
			$data = array('',$time,$hosts);
			array_push($info['data'],$data);
	
			$time = $time_act;
			
			
		}
		
		$info['showXLabel'] = true;
		$info['showYLabel'] = true;
		$info['XTickIncrement'] = 5000;
		$info['YTickIncrement'] = 1;
		
		$info['title'] = "Host Evolution";
		$info['data_type'] = "data-data";
		$info['graph_type'] = "lines";
		$info['is_legend'] = false;
		
		$graph = new Graph($info,800,200);
		$graph->draw();
	}
	
	function buildTaskTrace($username, $date, $trace)
	{
		$output = OUTPATH."/$username-trace-$date.txt";
		$file = new File($output, "trace");
		$traces = $file->parse();
		$parsed = $traces[$trace];
		
		$time = "0.0";
		$evn_enter = false;
		$evn_start = false;
		$evn_finish = false;
		$evn_new_host = false;
		$evn_stop_host = false;
		$evn_schedule = false;
		
		$info['data'] = array();
		$i = 0;
		$entered = 0;
		$started = 0;
		$finished = 0;
		while($i < count($parsed))
		{
			
			$time_act = ereg_replace("^0+", "", $parsed[$i]["time"]);
			while($time_act == $time && $i<count($parsed))
			{
				switch($parsed[$i]["event"])
				{
					
					case "EVN_ENTER":
										$evn_enter = true;
										$entered++;
										break;
										
					case "EVN_START":
										$evn_start = true;
										$started++;
										break;
					case "EVN_FINISH":
										$finished++;
										$evn_finish = true;
										break;
										
				}
				
				$time_act = ereg_replace("^0+", "", $parsed[$i]["time"]);
				$i++;
			}
			$data = array('',$time,$entered,$started,$finished);
			array_push($info['data'],$data);
	
			$time = $time_act;
			
			
		}
		
		$info['showXLabel'] = true;
		$info['showYLabel'] = true;
		$info['XTickIncrement'] = 5000;
		$info['YTickIncrement'] = 1;
		
		$info['title'] = "Task Evolution";
		$info['data_type'] = "data-data";
		$info['graph_type'] = "lines";
		$info['is_legend'] = true;
		$info['legend'] = array("entered","started","finished");
	
		$graph = new Graph($info,800,600);
		$graph->draw();
	
	}
	
	
	if(isset($_GET["graph"]))
	{
		switch($_GET["graph"])
		{
			case "time1":
								buildBarsGraph($_GET["username"],$_GET["date"],array("Makespan","Flowtime","Total Potential Time"),array("Makespan","Flowtime","Total Potential Time"),"time");
								break;
			case "time2":	
								buildPieGraph($_GET["username"],$_GET["date"]);
								break;
			case "number_hosts":
								buildNumberHostGraph($_GET["username"],$_GET["date"]);
								break;
			case "host_utilization":
									buildBarsGraph($_GET["username"],$_GET["date"],array("min","max","avg","sd"),array("Min. Host Utilisation","Max. Host Utilisation","Avg. Host Utilisation","Sd. Host Utilisation"),"Host utilisation");
									break;
			case "queue":
								buildBarsGraph($_GET["username"],$_GET["date"],array("min","max","avg","sd"),array("Min.Global Queue Length","Max.Global Queue Length","Avg.Global Queue Length","Sd. Global Queue Length"),"Global Queue Length");
								break;
			case "wait":
								buildBarsGraph($_GET["username"],$_GET["date"],array("min","max","avg","sd"),array("Min.Wait Time","Max.Wait Time","Avg.Wait Time","Sd.Wait Time"),"Wait time");
								break;
			case "schedules":
								buildBarsGraph($_GET["username"],$_GET["date"],array("min","max","avg","sd"),array("Min.Schedules x Task","Max.Schedules x Task","Avg.Schedules x Task","Sd.Schedules x Task"),"Schedules per Task");
								break;
			case "activations":
								buildBarsGraph($_GET["username"],$_GET["date"],array("min","max","avg","sd"),array("Min.Activation Interval","Max.Activation Interval","Avg.Activation Interval","Sd. Activation Interval"),"Activations Interval");
								break;
			
			case "timeline_trace":
								buildTimeLineTrace($_GET["username"],$_GET["date"], $_GET["trace"]);
								break;
			case "host_trace":	
								buildHostTrace($_GET["username"],$_GET["date"], $_GET["trace"]);
								break;
			
			case "task_trace":	
								buildTaskTrace($_GET["username"],$_GET["date"], $_GET["trace"]);
								break;
			
			case "runs_time":
								buildRunsTimeGraph($_GET["username"],$_GET["date"]);
								break;
			
			case "runs_hosts":	
								buildBarsGraph($_GET["username"],$_GET["date"],array("min","max","avg","sd"),array("Minimum utilisation","Maximum utilisation","Average utilisation","Sd. utilisation"),"Host utilization");
								break;
																																																																																
																															
		}		
	}
	
?>