result = [];
elements = document.getElementsByClassName('jobsearch-SerpJobCard');
for (var element of elements)
    result.push(element.innerHTML);

result;