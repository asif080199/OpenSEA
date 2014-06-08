var wpNotesCommon;var wpNotesCommentModView;var wpNoteList;var wpNoteModel;(function($){var cookies=document.cookie.split(/;\s*/),cookie=false;for(i=0;i<cookies.length;i++){if(cookies[i].match(/^wp_api=/)){cookies=cookies[i].split('=');cookie=cookies[1];break;}}
wpNotesCommon={jsonAPIbase:'https://public-api.wordpress.com/rest/v1',hasUpgradedProxy:false,noteTypes:{comment:'comment',follow:'follow',like:['like','like_trap'],reblog:'reblog',trophy:['best_liked_day_feat','like_milestone_achievement','achieve_automattician_note','achieve_user_anniversary','best_followed_day_feat','followed_milestone_achievement'],'alert':['expired_domain_alert']},noteType2Noticon:{'like':'like','follow':'follow','comment_like':'like','comment':'comment','comment_pingback':'external','reblog':'reblog','like_milestone_achievement':'trophy','achieve_followed_milestone_note':'trophy','achieve_user_anniversary':'trophy','best_liked_day_feat':'milestone','best_followed_day_feat':'milestone','automattician_achievement':'trophy','expired_domain_alert':'alert','automattcher':'atsign'},createNoteContainer:function(note,id_prefix){var note_container=$('<div/>',{id:id_prefix+'-note-'+note.id,'class':'wpn-note wpn-'+note.type+' '+((note.unread>0)?'wpn-unread':'wpn-read')}).data({id:parseInt(note.id,10),type:note.type});var note_body=$('<div/>',{"class":"wpn-note-body wpn-note-body-empty"});var spinner=$('<div/>',{style:'position: absolute; left: 180px; top: 60px;'});note_body.append(spinner);spinner.spin('medium');note_container.append(this.createNoteSubject(note),note_body);return note_container;},createNoteSubject:function(note){var subj=$('<div/>',{"class":"wpn-note-summary"}).append($('<span/>',{"class":'wpn-noticon noticon noticon'+note.noticon}),$('<span/>',{"class":'wpn-icon no-grav',html:$('<img/>',{src:note.subject.icon,width:'24px',height:'24px',style:'display: inline-block; width: 24px; height: 24px; overflow-x: hidden; overflow-y: hidden;'})}),$('<span/>',{"class":'wpn-subject',html:note.subject.html}));return subj;},createNoteBody:function(note_model){var note_body=$('<div/>',{"class":"wpn-note-body"});var note=note_model.toJSON();var class_prefix='wpn-'+note.body.template;switch(note.body.template){case'single-line-list':case'multi-line-list':note_body.append($('<p/>').addClass(class_prefix+'-header').html(note.body.header));for(var i in note.body.items){var item=$('<div></div>',{'class':class_prefix+'-item '+class_prefix+'-item-'+i+
(note.body.items[i].icon?'':' '+class_prefix+'-item-no-icon')});if(note.body.items[i].icon){item.append($('<img/>',{"class":class_prefix+'-item-icon avatar avatar-'+note.body.items[i].icon_width,height:note.body.items[i].icon_height,width:note.body.items[i].icon_width,src:note.body.items[i].icon}));}
if(note.body.items[i].header){item.append($('<div></div>',{'class':class_prefix+'-item-header'}).html(note.body.items[i].header));}
if(note.body.items[i].action){switch(note.body.items[i].action.type){case'follow':var button=wpFollowButton.create(note.body.items[i].action);item.append(button);button.click(function(e){if($(this).children('a').hasClass('wpcom-follow-rest'))
wpNotesCommon.bumpStat('notes-click-action','unfollow');else
wpNotesCommon.bumpStat('notes-click-action','follow');return true;});break;default:console.error("Unsupported "+note.type+" action: "+note.body.items[i].action.type);break;}}
if(note.body.items[i].html){item.append($('<div></div>',{'class':class_prefix+'-item-body'}).html(note.body.items[i].html));}
note_body.append(item);}
if(note.body.actions){var note_actions=new wpNotesCommentModView({model:note_model});note_actions.render();note_body.append(note_actions.el);}
if(note.body.footer){note_body.append($('<p/>').addClass(class_prefix+'-footer').html(note.body.footer));}
break;case'big-badge':if(note.body.header){note_body.append($('<div/>').addClass(class_prefix+'-header').html(note.body.header));}
if(note.body.badge){note_body.append($('<div></div>',{'class':class_prefix+'-badge '}).append(note.body.badge));}
if(note.body.html!==''){note_body.append($('<div/>').addClass(class_prefix+'-footer').html(note.body.html));}
break;default:note_body.text('Unsupported note body template!');break;}
note_body.find('a[notes-data-click]').mousedown(function(e){var type=$(this).attr('notes-data-click');wpNotesCommon.bumpStat('notes-click-body',type);return true;});return note_body;},getNoteSubjects:function(query_params,success,fail){query_params.fields='id,type,unread,noticon,timestamp,subject';query_params.trap=true;return this.getNotes(query_params,success,fail);},getNotes:function(query_params,success,fail){return this.ajax({type:'GET',path:'/notifications/',data:query_params,success:success,error:fail});},markNotesSeen:function(timestamp){return this.ajax({type:'POST',path:'/notifications/seen',data:{time:timestamp}});},markNotesRead:function(unread_cnts){var query_args={};var t=this;for(var id in unread_cnts){if(unread_cnts[id]>0){query_args['counts['+id+']']=unread_cnts[id];}}
if(0===query_args.length){return(new $.Deferred()).resolve('no unread notes');}
return this.ajax({type:'POST',path:'/notifications/read',data:query_args,success:function(res){},error:function(res){}});},ajax:function(options){var t=this;var request={path:options.path,method:options.type};if(options.type.toLowerCase()=='post')
request.body=options.data;else
request.query=options.data;return $.Deferred(function(dfd){var makeProxyCall=function(){$.wpcom_proxy_request(request,function(response,statusCode){if(200==statusCode){if('function'==typeof options.success){options.success(response);}
return dfd.resolve(response);}
if('function'==typeof options.error){options.error(statusCode);}
else{console.error(statusCode);}
return dfd.reject(statusCode);});};if(t.hasUpgradedProxy){return makeProxyCall();}
return $.wpcom_proxy_request({metaAPI:{accessAllUsersBlogs:true}}).done(function(){t.hasUpgradedProxy=true;makeProxyCall();});});},bumpStat:function(group,names){if('undefined'!=typeof wpNotesIsJetpackClient&&wpNotesIsJetpackClient){var jpStats=['notes-menu-impressions','notes-menu-clicks'];if(_.contains(jpStats,group)){names=names.replace(/(,|$)/g,'-jetpack$1');}}
new Image().src=document.location.protocol+'//stats.wordpress.com/g.gif?v=wpcom-no-pv&x_'+group+'='+names+'&baba='+Math.random();},getKeycode:function(key_event){key_event=key_event||window.event;if(key_event.target)
element=key_event.target;else if(key_event.srcElement)
element=key_event.srcElement;if(element.nodeType==3)
element=element.parentNode;if(key_event.ctrlKey===true||key_event.altKey===true||key_event.metaKey===true)
return false;var keyCode=(key_event.keyCode)?key_event.keyCode:key_event.which;if(keyCode&&(element.tagName=='INPUT'||element.tagName=='TEXTAREA'||element.tagName=='SELECT'))
return false;if(keyCode&&element.contentEditable=="true")
return false;return keyCode;}};wpNoteModel=Backbone.Model.extend({defaults:{summary:"",unread:true},initialize:function(){},markRead:function(){var unread_cnt=this.get('unread');if(Boolean(parseInt(unread_cnt,10))){var notes={};notes[this.id]=unread_cnt;wpNotesCommon.markNotesRead(notes);wpNotesCommon.bumpStat('notes-read-type',this.get('type'));}},loadBody:function(){wpNotesCommon.createNoteBody(this);},reload:function(){var t=this;var fields='id,type,unread,noticon,subject,body,date,timestamp';if('comment'==t.get('type')){fields+=',approval_status,has_replied';}
return wpNotesCommon.getNotes({fields:fields,trap:true,ids:[t.get('id')]},function(res){var notes=res.notes;if(typeof notes[0]!=='undefined'){t.set(notes[0]);}},function(){});},resize:function(){this.trigger('resize');}});wpNoteList=Backbone.Collection.extend({model:wpNoteModel,lastMarkedSeenTimestamp:false,newNotes:false,maxNotes:false,loading:false,hasLoaded:false,allBodiesLoaded:false,comparator:function(note){return-note.get('timestamp');},addNotes:function(notes){notes=_.filter(notes,function(note){return typeof(note.subject)==="object";});var models=_.map(notes,function(o){return new wpNoteModel(o);});this.add(models);this.sort();if(this.maxNotes){while(this.length>this.maxNotes){this.pop();}}
this.trigger('loadNotes:change');},getMostRecentTimestamp:function(){if(!this.length){return false;}
this.sort();return parseInt(this.at(0).get('timestamp'),10);},loadNotes:function(query_args){var t=this;t.loading=true;t.trigger('loadNotes:beginLoading');var fields=query_args.fields;var number=parseInt(query_args.number,10);var before=parseInt(query_args.before,10);var since=parseInt(query_args.since,10);var timeout=parseInt(query_args.timeout,10)||7000;var type='undefined'==typeof query_args.type?null:query_args.type;var unread='undefined'==typeof query_args.unread?null:query_args.unread;query_args={timeout:timeout};if(!fields){fields='id,type,unread,noticon,subject,body,date,timestamp';}
if(isNaN(number)){number=9;}
if(!isNaN(before)){query_args.before=before;}
if(!isNaN(since)){query_args.since=since;}
if(unread!==null){query_args.unread=unread;}
if(type!==null&&type!="unread"&&type!="latest"){query_args.type=type;}
query_args.number=number;query_args.fields=fields;query_args.trap=true;return wpNotesCommon.getNotes(query_args).done(function(res){var qt;var notes=res.notes;var notes_changed=false;if(!t.lastMarkedSeenTimestamp||(res.last_seen_time>t.lastMarkedSeenTimestamp)){notes_changed=true;t.lastMarkedSeenTimestamp=parseInt(res.last_seen_time,10);}
for(var idx in notes){var note_model=t.get(notes[idx].id);if(note_model){if(typeof(notes[idx].subject)!='object'){t.remove(notes[idx].id);notes_changed=true;continue;}
if(type){qt=note_model.get('queried_types')||{};qt[type]=true;notes[idx].queried_types=qt;}
note_model.set(notes[idx]);}
else{if(typeof(notes[idx].subject)!='object'){continue;}
if(type){qt={};qt[type]=true;notes[idx].queried_types=qt;}
note_model=new wpNoteModel(notes[idx]);t.add(note_model);}
if(!note_model.has('body'))
t.allBodiesLoaded=false;notes_changed=true;}
if(t.maxNotes){while(t.length>t.maxNotes){t.pop();}}
if(notes_changed){t.sort();t.trigger('loadNotes:change');}
t.loading=false;t.hasLoaded=true;t.trigger('loadNotes:endLoading');}).fail(function(e){t.loading=false;t.trigger('loadNotes:failed');});},loadNoteBodies:function(filter){var t=this;if(t.allBodiesLoaded){return(new $.Deferred()).resolve();}
var ids=t.getNoteIds(filter);if(0==ids.length){return(new $.Deferred()).reject();}
var doneFunc=function(res){var notes=res.notes;for(var idx in notes){if(typeof(notes[idx].subject)!='object'){continue;}
var note_model=t.get(notes[idx].id);if(note_model){note_model.set(notes[idx]);}else{note_model=new wpNoteModel(notes[idx]);t.add(note_model);}}};var failFunc=function(e){if(typeof console!='undefined'&&typeof console.error=='function')
console.error('body loading error!');}
var deferreds=[];var count=3
for(var i=0;i<count;i++){if(typeof ids[i]=='undefined')
break;var query_params={};query_params.fields='id,type,unread,noticon,timestamp,subject,body,meta';query_params.trap=true;query_params['ids['+i+']']=ids[i];deferreds.push(wpNotesCommon.getNotes(query_params)
.done(doneFunc)
.fail(failFunc));}
if(ids.length>count){var query_params={};query_params.fields='id,type,unread,noticon,timestamp,subject,body,meta';query_params.trap=true;for(var i=count;i<ids.length;i++)
query_params['ids['+i+']']=ids[i];deferreds.push(wpNotesCommon.getNotes(query_params)
.done(doneFunc)
.fail(failFunc));}
var all_xhr=$.when.apply(null,deferreds);all_xhr.done(function(){if(typeof filter!='function'){t.allBodiesLoaded=true;}});return all_xhr;},markNotesSeen:function(){var t=this,mostRecentTs=t.getMostRecentTimestamp();if(mostRecentTs>this.lastMarkedSeenTimestamp){wpNotesCommon.markNotesSeen(mostRecentTs).done(function(){t.lastMarkedSeenTimestamp=false;});}},unreadCount:function(){return this.reduce(function(num,note){return num+(note.get('unread')?1:0);},0);},numberNewNotes:function(){var t=this;if(!t.lastMarkedSeenTimestamp)
return 0;return t.getNewNotes().length;},getNewNotes:function(){var t=this;return t.filter(function(note){return(note.get('timestamp')>t.lastMarkedSeenTimestamp);});},getUnreadNotes:function(){return this.filter(function(note){return Boolean(parseInt(note.get("unread"),10));});},getNotesOfType:function(typeName){var t=this;switch(typeName){case'unread':return t.getUnreadNotes();case'latest':return t.filter(function(note){var qt=note.get('queried_types');return'undefined'!=typeof qt&&'undefined'!=typeof qt.latest&&qt.latest;});default:return t.filter(function(note){var note_type=note.get("type");if("undefined"==typeof wpNotesCommon.noteTypes[typeName]){return false;}
else if("string"==typeof wpNotesCommon.noteTypes[typeName]){return typeName==note_type;}
var len=wpNotesCommon.noteTypes[typeName].length;for(var i=0;i<len;i++){if(wpNotesCommon.noteTypes[typeName][i]==note_type){return true;}}
return false;});}},getNoteIds:function(filter){if(typeof filter!='function')
filter=function(){return true;};return _.pluck(this.filter(filter),'id');}});wpNotesCommentModView=Backbone.View.extend({mode:'buttons',commentNeedsApproval:false,actionIDMap:{},events:{'click .wpn-replyto-comment-button-open a':'openReply','click .wpn-comment-reply-button-close':'closeReply','click .wpn-comment-reply-button-send':'sendReply','click .wpn-approve-comment-button a':'clickModComment','click .wpn-unapprove-comment-button a':'clickModComment','click .wpn-spam-comment-button a':'clickModComment','click .wpn-unspam-comment-button a':'clickModComment','click .wpn-trash-comment-button a':'clickModComment','click .wpn-untrash-comment-button a':'clickModComment'},templateButton:'\
			<span class="{{class_name}}">\
				<a href="#" title="{{title_text}} Keyboard shortcut: {{keytext}}" data-action-type="{{data_action_type}}">{{#bold}}<b>{{/bold}}{{text}}{{#bold}}</b>{{/bold}}</a>\
			</span>\
		',templateReply:'\
			<div class="wpn-note-comment-reply"> \
				<h5>{{reply_header_text}}</h5>\
				<textarea class="wpn-note-comment-reply-text" rows="5" cols="45" name="wpn-note-comment-reply-text"></textarea>\
				<p class="wpn-comment-submit">\
					<span class="wpn-comment-submit-waiting" style="display: none;"></span>\
				<span class="wpn-comment-submit-error" style="display:none;">Error!</span>\
				<a href="#" class="wpn-comment-reply-button-send alignright">{{submit_button_text}}</a>\
				<a href="#" class="wpn-comment-reply-button-close alignleft">_</a>\
				</p>\
			</div>\
		',initialize:function(){var t=this;_.bindAll(this,'render');t.setElement($('<div class="wpn-note-comment-actions" />'));this.model.bind('change',this.render,this);$(document).keydown(function(key_event){if(t.$el.is(':hidden')){return;}
if(t.mode!='buttons'){return;}
var keyCode=wpNotesCommon.getKeycode(key_event);if(!keyCode){return;}
if(keyCode==82){if(typeof t.actionIDMap['replyto-comment']!='undefined')
t.openReply(key_event);return false;}
if(keyCode==65){if(typeof t.actionIDMap['approve-comment']!='undefined')
t.modComment('approve-comment');else if(typeof t.actionIDMap['unapprove-comment']!='undefined')
t.modComment('unapprove-comment');return false;}
if(keyCode==83){if(typeof t.actionIDMap['spam-comment']!='undefined')
t.modComment('spam-comment');else if(typeof t.actionIDMap['unspam-comment']!='undefined')
t.modComment('unspam-comment');return false;}
if(keyCode==84){if(typeof t.actionIDMap['trash-comment']!='undefined')
t.modComment('trash-comment');else if(typeof t.actionIDMap['untrash-comment']!='undefined')
t.modComment('untrash-comment');return false;}});},render:function(){var t=this;t.$el.empty();if(t.mode=='buttons'){t.$el.append.apply(this.$el,this.createActions());}else{t.$el.html(t.createReplyBox());t.$('textarea').focus();}
t.delegateEvents();return t;},createActions:function(){var actions=this.model.get('body').actions;var t=this;var elements=[];this.actionIDMap={};var cnt=0;_.forEach(actions,function(action){var keytext;t.actionIDMap[action.type]=cnt++;var button_data={"ajax_url":action.params.url,"title_text":action.params.title_text||action.params.button_title_text,"data_action_type":action.type,"text":action.params.button_text||action.params.text};switch(action.type){case'replyto-comment':button_data=$.extend({keytext:'[r]',class_name:'wpn-'+action.type+'-button-open'},button_data);elements.push(Mustache.render(t.templateButton,button_data));elements.push(' | ');break;case'approve-comment':button_data=$.extend({keytext:'[a]',bold:true,class_name:'wpn-'+action.type+'-button'},button_data);t.commentNeedsApproval=true;elements.push(Mustache.render(t.templateButton,button_data));elements.push(' | ');break;case'unapprove-comment':keytext='[a]';case'spam-comment':keytext=(typeof keytext=='undefined')?'[s]':keytext;case'unspam-comment':keytext=(typeof keytext=='undefined')?'[s]':keytext;case'trash-comment':keytext=(typeof keytext=='undefined')?'[t]':keytext;case'untrash-comment':keytext=(typeof keytext=='undefined')?'[t]':keytext;button_data=$.extend({keytext:keytext,class_name:'wpn-'+action.type+'-button'},button_data);elements.push(Mustache.render(t.templateButton,button_data));elements.push(' | ');break;}});elements=elements.slice(0,-1);elements.push($('<span/>',{'class':"wpn-comment-mod-waiting",style:"display:none;"}));return elements;},createReplyBox:function(){var action=this.model.get('body').actions[this.actionIDMap['replyto-comment']];var blog_id=action.params.blog_id||0;var comment_id=this.model.id||0;var element=Mustache.render(this.templateReply,{reply_header_text:action.params.reply_header_text,submit_button_text:action.params.submit_button_text});return element;},closeReply:function(ev){if(ev)
ev.preventDefault();this.mode='buttons';this.model.currentReplyText=this.$el.children('.wpn-note-comment-reply').children('.wpn-note-comment-reply-text').val();this.render();this.model.resize();},openReply:function(ev){ev.preventDefault();this.mode='reply';this.render();this.$el.children('.wpn-note-comment-reply').children('.wpn-note-comment-reply-text').val(this.model.currentReplyText);this.model.resize();},sendReply:function(ev){ev.preventDefault();var t=this;var comment_reply_el=t.$el.children('.wpn-note-comment-reply');var action=t.model.get('body').actions[t.actionIDMap['replyto-comment']];t.model.currentReplyText=comment_reply_el.children('.wpn-note-comment-reply-text').val();comment_reply_el.children('.wpn-comment-submit').children('.wpn-comment-submit-error').hide();comment_reply_el.children('.wpn-comment-submit').children('.wpn-comment-submit-waiting').show();var	blog_id=action.params.blog_id||0,comment_id=action.params.comment_id||0,content=t.model.currentReplyText||0;if(!(blog_id&&comment_id&&content))
return error.log('Invalid sendReply params');$('.wpn-comment-submit-waiting').spin('small');wpNotesCommon.bumpStat('notes-click-action','replyto-comment');var doSend=function(){return wpNotesCommon.ajax({type:'POST',path:'/sites/'+blog_id+'/comments/'+comment_id+'/replies/new',data:{content:content},success:function(r){if(typeof r=='string'){t.errorReply(r);return false;}
t.closeReply();t.model.currentReplyText='';t.model.reload().done(function(){if(!t.model.get('has_replied')){var tries=0;t.replyCommentInterval=setInterval(function(){t.model.reload().done(function(){if(t.model.get('has_replied')||tries++>=10){clearInterval(t.replyCommentInterval);}});},3000);}});},error:function(r){t.errorReply(r);}});};if(t.commentNeedsApproval){return t.modComment('approve-comment').done(doSend);}else
return doSend();},errorReply:function(r){var t=this;var er=r;if(typeof r=='object'){if(r.responseText){o=$.parseJSON(r.responseText);er=o.error+': '+o.message;}else if(r.statusText){er=r.statusText;}else{er='Unknown Error';}}
var comment_reply_el=t.$el.children('.wpn-note-comment-reply');comment_reply_el.children('.wpn-comment-submit').children('.wpn-comment-submit-waiting').hide();if(er)
comment_reply_el.children('.wpn-comment-submit').children('.wpn-comment-submit-error').text(er).show();},clickModComment:function(ev){var t=this;ev.preventDefault();var type=$(ev.currentTarget).data('action-type');if('undefined'==typeof type)
return(new $.Deferred()).reject('Undefined action-type');return t.modComment(type);},modComment:function(type){var t=this;var action_id=t.actionIDMap[type];var action=t.model.get('body').actions[t.actionIDMap[type]];if('undefined'==typeof action_id)
return(new $.Deferred()).reject('Undefined action_id');t.$(' .wpn-comment-mod-waiting').show().spin('small');wpNotesCommon.bumpStat('notes-click-action',type);var oldStatus=t.model.get('approval_status');clearInterval(t.modCommentInterval);return wpNotesCommon.ajax({type:'POST',path:action.params.rest_path,data:action.params.rest_body,success:function(r){t.model.reload().done(function(){if(oldStatus==t.model.get('approval_status')){var tries=0;t.modCommentInterval=setInterval(function(){t.model.reload().done(function(){if(oldStatus!=t.model.get('approval_status')||tries++>=10){clearInterval(t.modCommentInterval);}});},3000);}});},error:function(r){console.error('Comment moderation error');t.model.reload();}});}});})(jQuery);