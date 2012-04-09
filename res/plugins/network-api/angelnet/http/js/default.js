$(function(){
var Page = {
    Init : function(){
        $("body").css("height",$(window).height());
        $("#loading").text("the future is now.").css("color","#fff");
        setTimeout(function(){
            $("#loading").show("puff",1500);
            $("body").animate({backgroundColor: "#fff", color: "#000"},2000);
            setTimeout(function(){
                $("#loading").hide("puff",1000);
                setTimeout(function(){
                    $("body").css("background","url('img/fav.png') no-repeat bottom right #fff");
                    Page.showLoading("Initializing...");
                    setTimeout(function(){Page.showLoading("Welcome");},800);
                },1100);
            },3500);            
        },500);
    },
    _slTimeout : 0,
    showLoading : function(text){
        if (text != undefined) $("#loading").text(text);
            
        if (Page._slTimeout == 0) $("#loading").show("blind",500);
        else $("#loading").show("bounce",500);
        
        clearTimeout(Page._slTimeout);
        Page._slTimeout = setTimeout(function(){
            $("#loading").hide("fade",500);
        },4000);
    }
};

Page.Init();
});
