<?php

include 'dbinfo.php';

$db = mssql_connect('red',$dbuser,$dbpwd);

mssql_select_db('mdsplus');

if ($_GET["platform"] != "")
{
  if ($_GET["firstname"] == "")
  {
    printf("<HTML>\n<title>Need First Name</title>\nFirst name missing.<P>\n");
    printf("Please hit the \"BACK\" button and fill in all fields on the form before hitting the \"Download Now\" button.\n</HTML>\n");
  }
  else if ($_GET["lastname"] == "")
  {
    printf("<HTML>\n<title>Need Last Name</title>\nLast name missing.<P>\n");
    printf("Please hit the \"BACK\" button and fill in all fields on the form before hitting the \"Download Now\" button.\n</HTML>\n");
  }
  else if ($_GET["email"] == "")
  {
    printf("<HTML>\n<title>Need Email Address</title>\nEmail address missing.<P>\n");
    printf("Please hit the \"BACK\" button and fill in all fields on the form before hitting the \"Download Now\" button.\n</HTML>\n");
  }
  else if ($_GET["comment"] == "")
  {
    printf("<HTML>\n<title>Need Comment</title>\nComment missing.<P>\n");
    printf("Please hit the \"BACK\" button and fill in all fields on the form before hitting the \"Download Now\" button.\n</HTML>\n");
  }
  else if ($_GET["platform"] == "0")
  {
    printf("<HTML>\n<title>Select product</title>\n");
    printf("Please hit the \"BACK\" button and select a product to download before hitting the \"Download Now\" button.\n</HTML>\n");
  }
  else
  {
    setcookie("firstname",$_GET["firstname"],time()+60*60*24*30);
    setcookie("lastname",$_GET["lastname"],time()+60*60*24*30);
    setcookie("email",$_GET["email"],time()+60*60*24*30);
    setcookie("comment",$_GET["comment"],time()+60*60*24*30);
    $q=mssql_query("select url from kits where id='" .  $_GET["platform"] . "'");
    $ans=mssql_fetch_row($q);
    $insert_query = "insert into downloads (kit_id,remote_addr,firstname,lastname,email,comment) values (";
    $insert_query = $insert_query .  $_GET["platform"]; 
    $insert_query = $insert_query .  ",'" . $_SERVER["remote_addr"];
    $insert_query = $insert_query .  "','" . str_replace("'","''",$_GET["firstname"]);
    $insert_query = $insert_query .  "','" . str_replace("'","''",$_GET["lastname"]);
    $insert_query = $insert_query .  "','" . str_replace("'","''",$_GET["email"]);
    $insert_query = $insert_query .  "','" . str_replace("'","''",$_GET["comment"]) . "')";
    $q=mssql_query($insert_query);
    header("Location: " . $ans[0]);
  }
}
else
{
?>

<HTML>
<head>
<link rel="stylesheet" href="mdsplus_css.css" type="text/css">
<title>MDSplus Download</title>
<META http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
</head>
<?php apache_setenv("SUBHEADING","Software"); ?>
<?php virtual("../heading.shtml"); ?>
<table cellpadding="5" cellspacing="0" border="0">
<tr valign="top">
<td>
<table>
   <?php virtual("menu.shtml"); ?>
</table>
</td>
<td>
<H2>Download MDSplus installation kit</H2>

<P>
<FORM METHOD="get" ACTION="download.php">
<INPUT type="hidden" name="Selected" value="1">
Please provide us with some information about you and how you will be using MDSplus.
Note this information will not be made publicly available but will help us in keeping
track of the distribution of MDSplus.
</P>
<P></P>
<P>
<TABLE>
<TR><TD>First name:</TD><TD><INPUT TYPE="text" NAME="firstname" size=40 value=<?php echo $_COOKIE["firstname"]; ?>></TD></TR>
<TR><TD>Last name:</TD><TD><INPUT TYPE="text" NAME="lastname" size=40 value=<?php echo $_COOKIE["lastname"]; ?>></TD></TR>
<TR><TD>Email address:</TD><TD><INPUT TYPE="text" NAME="email" SIZE="40" value=<?php echo $_COOKIE["email"]; ?>></TD></TR>
<TR><TD>Brief comment on usage:</TD><TD><INPUT TYPE="text" NAME="comment" SIZE="40" value=<?php echo $_COOKIE["comment"]; ?>></TD></TR>
<TR><TD>Select Kit:</TD><TD><SELECT name="platform">
<OPTION SELECTED TYPE="RADIO" value=0><b>------------------------- Please select kit to download -------------------------</b></OPTION>
<?php
   $q = mssql_query("select description,id,creation from kits where active = 1 order by description");
   while($ans=mssql_fetch_row($q))
   {
     printf("<OPTION TYPE=\"RADIO\" value=" . $ans[1] . ">" . $ans[2] . " --- " . $ans[0] . "</OPTION>\n");
   }
?>
<OPTION TYPE="RADIO" value=0>---------------------------- Previous releases --------------------------------</OPTION>
<?php
   $q = mssql_query("select description,id,creation from kits where active = 0 order by description");
   while($ans=mssql_fetch_row($q))
   {
     printf("<OPTION TYPE=\"RADIO\" value=" . $ans[1] . ">" . $ans[2] . " --- " . $ans[0] . "</OPTION>\n");
   }
?>
</SELECT></TD></TR>
</TABLE>
<P>
<INPUT type="submit" value="Download Now">
<INPUT type="reset" value="Reset">
</P>
<?php
}
mssql_close();
?>
</FORM>
</TD>
</TR>
</TABLE>
</HTML>
