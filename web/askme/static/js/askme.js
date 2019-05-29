
function getCookie(name) {
    var cookieValue = null;
    if (document.cookie && document.cookie !== '') {
        var cookies = document.cookie.split(';');
        for (var i = 0; i < cookies.length; i++) {
            var cookie = jQuery.trim(cookies[i]);
            // Does this cookie string begin with the name we want?
            if (cookie.substring(0, name.length + 1) === (name + '=')) {
                cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
                break;
            }
        }
    }
    return cookieValue;
}

$(document).ready(function() {
    $(".js-like-question").on("click", function() {
        var $this = $(this);
        $.ajax({
            url: "/like_article/",
            method: "POST",
            dataType: "json",
            data: {
                "question_id": $this.data("question_id"),
                "csrfmiddlewaretoken": getCookie("csrftoken")
            }
        }).done(function(data) {
            console.log(data);
        });
        return false;
    });
    $(".js-like-answer").on("click", function() {
        var $this = $(this);
        $.ajax({
            url: "/like_answer/",
            method: "POST",
            dataType: "json",
            data: {
                "answer_id": $this.data("answer_id"),
                "csrfmiddlewaretoken": getCookie("csrftoken")
            }
        }).done(function(data) {
            console.log(data);
        });
        return false;
    });
    $(".js-dislike-question").on("click", function() {
        var $this = $(this);
        $.ajax({
            url: "/dislike_article/",
            method: "POST",
            dataType: "json",
            data: {
                "question_id": $this.data("question_id"),
                "csrfmiddlewaretoken": getCookie("csrftoken")
            }
        }).done(function(data) {
            console.log(data);
        });
        return false;
    });
    $(".js-dislike-answer").on("click", function() {
        var $this = $(this);
        $.ajax({
            url: "/dislike_answer/",
            method: "POST",
            dataType: "json",
            data: {
                "answer_id": $this.data("answer_id"),
                "csrfmiddlewaretoken": getCookie("csrftoken")
            }
        }).done(function(data) {
            console.log(data);
        });
        return false;
    });
});