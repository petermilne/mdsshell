ADDING DOCUMENTATION TO MDSPLUS.ORG
-----------------------------------

To add documentation, just edit the file to be updated and add your
content between these comment tags:

  <!-- BEGIN: Put Content Here -->
  <!-- END: Put Content Here -->

That's all there is to it.

TIPS:

  1. When adding content, leave the page open in your web browser
     and click refresh to make sure the page looks like you want
     it to look.  That way you can make a few changes in your text
     editor, click refresh to make sure they look good, and proceed.

  2. Check the look of your web page using multiple browsers (on 
     multiple platforms, if at all possible).  Netscape seems to
     be the most finicky browser, (fewer fonts, fewer HTML tags,
     less CSS support) so if it looks OK in Netscape it will
     probably look OK in IE, Mozilla, etc.

You can change the appearance of the entire site by tweaking
the mdsplus_css.css file.  The MDSplus header at the top of each
page is also in one place, so changing that file will have
a global effect.  There is one menu for each subdirectory of
the web site, so to change menus you will need to edit the
menu.shtml file in the directory you are working with.

Once you have made modifications to the MDSplus documentation and
checked your modifications back into the cvs repository using 'cvs commit',
you can update the contents of the MDSplus web site by browsing to the
page: http://www.mdsplus.org/cgi-bin/cvs_update_html


