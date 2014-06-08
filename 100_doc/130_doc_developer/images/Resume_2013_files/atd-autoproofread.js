var AtD_proofread_click_count=0;function AtD_submit_check(e){if(AtD_proofread_click_count>0)
return;e.stopImmediatePropagation();e.preventDefault();if(typeof(tinyMCE)!='undefined'&&tinyMCE.activeEditor&&!tinyMCE.activeEditor.isHidden()){tinyMCE.activeEditor.execCommand('mceWritingImprovementTool',AtD_submit_check_callback);}else{AtD_restore_if_proofreading();AtD_check(AtD_submit_check_callback);}}
function AtD_submit_check_callback(count){count=count||0;AtD_unbind_proofreader_listeners();if(0==count||1<AtD_proofread_click_count){AtD_update_post();}else if(-1==count){alert(AtD.getLang('message_server_error','There was a problem communicating with the Proofreading service. Try again in one minute.'));AtD_update_post();}else{var original_post_status=jQuery('#original_post_status').val()
var message;if(original_post_status=='publish')
message=AtD.getLang('dialog_confirm_post_publish','The proofreader has suggestions for this post. Are you sure you want to publish it?\n\nPress OK to publish your post, or Cancel to view the suggestions and edit your post.');else
message=AtD.getLang('dialog_confirm_post_update','The proofreader has suggestions for this post. Are you sure you want to update it?\n\nPress OK to update your post, or Cancel to view the suggestions and edit your post.');if(confirm(message)){AtD_update_post();}else{AtD_bind_proofreader_listeners();AtD_kill_autoproofread();}
jQuery('#publish').removeClass('button-primary-disabled');jQuery('#ajax-loading').hide();}}
function AtD_kill_autoproofread(){jQuery('#publish').unbind('click.AtD_submit_check');}
function AtD_update_post(){if(typeof(tinyMCE)=='undefined'||!tinyMCE.activeEditor||tinyMCE.activeEditor.isHidden())
AtD_restore_if_proofreading();jQuery('#publish').unbind('click.AtD_submit_check').click();}
jQuery(document).ready(function($){var orig_status=$('#original_post_status').val();if(typeof AtD_check_when!='undefined'&&$('#content').length&&((orig_status!='publish'&&AtD_check_when.onpublish)||((orig_status=='publish'||orig_status=='schedule')&&AtD_check_when.onupdate)))
$('#publish').bind('click.AtD_submit_check',AtD_submit_check);});