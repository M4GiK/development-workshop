<?
	session_start();
	//session_register("admin");
	if(!isset($_SESSION["admin"]))
	{	
		header("location:../login.php");
		exit();
	}
?>			
<?
	include "../config.php";
	include "../../management/newsManager.php";
	$action = $_GET["action"];
	$new = $_GET["new"];
	
	
	$newsm = new newsManager();
	switch($action)
	{
		case "edit":
					//echo "section:$section subsection:$subsection<br/>";
					$info = $newsm->getNew($new);
					//print_r($info);
					
					break;
	}

?>
<form action="saveNew.php" method="post" name="form1" id="form1">
  <table width="80%" border="1" align="center" cellspacing="0" bordercolor="#000000">
    <tr>
      <td width="14%" align="center" valign="top" bordercolor="#333333" bgcolor="#CCCCCC"><strong>titular_es</strong></td>
      <td width="86%" bordercolor="#333333"><input type="text" name="titular_es" value="<? echo $info["titular_es"];?>" /> 
      <br /></td>
    </tr>
    <tr>
      <td align="center" valign="top" bordercolor="#333333" bgcolor="#CCCCCC"><strong>titular_en</strong></td>
      <td bordercolor="#333333"><input type="text" name="titular_en" value="<? echo $info["titular_en"]; ?>" />
      <br /></td>
    </tr>
    <tr>
      <td align="center" valign="top" bordercolor="#333333" bgcolor="#CCCCCC"><strong>titular_ca</strong></td>
      <td bordercolor="#333333"><input type="text" name="titular_ca" value="<? echo $info["titular_ca"];?>"/>
          <br /></td>
    </tr>
	 <tr>
      <td align="center" valign="top" bordercolor="#333333" bgcolor="#CCCCCC"><strong>fecha_exp </strong></td>
      <td valign="top" bordercolor="#333333">Dia: 
        <select name="day" size="1" id="day">
        <?
		   		$daysel = date("j",$info["fecha_exp"]);
				for($i = 1; $i <= 31; $i++)
				{
					if ($i != $daysel)
						echo "<option value=\"$i\">$i</option>";
					else
						echo "<option value=\"$i\" selected=\"selected\">$i</option>";
				}
		   ?>
        </select>
       Mes: 
       <select name="month" id="month">
         <?
		   		$monthsel = date("n",$info["fecha_exp"]);
				for($i = 1; $i <= 12; $i++)
				{
					if ($i != $monthsel)
						echo "<option value=\"$i\">$i</option>";
					else
						echo "<option value=\"$i\" selected=\"selected\">$i</option>";
				}
		   ?>
       </select>
        A&ntilde;o: 
        <input name="year" type="text" id="year" size="4" maxlength="4" value="<? echo date("Y",$info["fecha_exp"]);?>" />
         Hora: 
         <select name="hour" id="hour">
           <?
		   		$hoursel = date("H",$info["fecha_exp"]);
				for($i = 0; $i < 24; $i++)
				{
					if ($i != $hoursel)
						echo "<option value=\"$i\">$i</option>";
					else
						echo "<option value=\"$i\" selected=\"selected\">$i</option>";
				}
		   ?>
        </select>
         <br />
       <br /></td>
	 </tr>
    <tr>
      <td align="center" valign="top" bordercolor="#333333" bgcolor="#CCCCCC"><strong>cuerpo_es</strong></td>
      <td valign="top" bordercolor="#333333"><textarea name="cuerpo_es" cols="80" rows="10"><? echo $info["cuerpo_es"];?></textarea>
      <br /></td>
    </tr>
    <tr>
      <td align="center" valign="top" bordercolor="#333333" bgcolor="#CCCCCC"><strong>cuerpo_en</strong></td>
      <td valign="top" bordercolor="#333333"><textarea name="cuerpo_en" cols="80" rows="10"><? echo $info["cuerpo_en"];?></textarea>
      <br /></td>
    </tr>
    <tr>
      <td align="center" valign="top" bordercolor="#333333" bgcolor="#CCCCCC"><strong>cuerpo_ca</strong></td>
      <td valign="top" bordercolor="#333333"><textarea name="cuerpo_ca" cols="80" rows="10"><? echo $info["cuerpo_ca"];?></textarea>
      <br /></td>
    </tr>
    <tr>
      <td align="center" valign="top" bordercolor="#333333" bgcolor="#CCCCCC"><strong>fecha </strong></td>
      <td bordercolor="#333333"><br />
      <? echo $info["fecha"];?>
	  <input type="hidden" name="new" value="<? echo $new;?>"/>
	  <input type="hidden" name="action" value="<? echo $action;?>"/>
	  </td>
    </tr>
    <tr>
      <td align="center" valign="top" bordercolor="#333333" bgcolor="#CCCCCC">&nbsp;</td>
      <td align="center" bordercolor="#333333"><input type="submit" name="Submit" value="guardar" /></td>
    </tr>
  </table>
  <a href="index.php">volver</a>

</form>

