var lastBtnName = document.getElementsByClassName('pagination-list')[0].
    lastElementChild.firstChild.getAttribute('aria-label')

if (lastBtnName == "Next") {
    document.getElementsByClassName('pagination-list')[0].lastElementChild.firstChild.click()
    "ok";
} else if (lastBtnName !== null) {
    "completed"
} else {
    "error"
}

/*
 made for:
 <ul class="pagination-list"><li><a href="/jobs?q=qt&amp;l=Daly+City%2C+CA&amp;radius=100&amp;start=30" aria-label="Previous" data-pp="gQAtAAABdXXIGwUAAAABkpkb3wAiAQEBCAK78XoeFYvmcHghxzuOfRd9gpZ3dRjW_tLhbOcLiAAA" onmousedown="addPPUrlParam &amp;&amp; addPPUrlParam(this);" rel="nofollow"><span class="pn"><span class="np"><svg width="24" height="24" fill="none"><path d="M15.41 7.41L14 6l-6 6 6 6 1.41-1.41L10.83 12l4.58-4.59z" fill="#2D2D2D"></path></svg></span></span></a></li><li><a href="/jobs?q=qt&amp;l=Daly+City%2C+CA&amp;radius=100&amp;start=20" aria-label="3" data-pp="gQAeAAABdXXIGwUAAAABkpkb3wAeAQBSencdyiXyYEnB4cTycrw3pRnrNYSAxkVfVH1EAAA" onmousedown="addPPUrlParam &amp;&amp; addPPUrlParam(this);" rel="nofollow"><span class="pn">3</span></a></li><li><a href="/jobs?q=qt&amp;l=Daly+City%2C+CA&amp;radius=100&amp;start=30" aria-label="4" data-pp="gQAtAAABdXXIGwUAAAABkpkb3wAiAQEBCAK78XoeFYvmcHghxzuOfRd9gpZ3dRjW_tLhbOcLiAAA" onmousedown="addPPUrlParam &amp;&amp; addPPUrlParam(this);" rel="nofollow"><span class="pn">4</span></a></li><li><b aria-current="true" aria-label="5" tabindex="0">5</b></li><li><a href="/jobs?q=qt&amp;l=Daly+City%2C+CA&amp;radius=100&amp;start=50" aria-label="6" data-pp="gQBLAAABdXXIGwUAAAABkpkb3wAUAQAfJhEOKm1MmXn50qB8IzbR3BIAAA" onmousedown="addPPUrlParam &amp;&amp; addPPUrlParam(this);" rel="nofollow"><span class="pn">6</span></a></li><li><a href="/jobs?q=qt&amp;l=Daly+City%2C+CA&amp;radius=100&amp;start=50" aria-label="Next" data-pp="gQBLAAABdXXIGwUAAAABkpkb3wAUAQAfJhEOKm1MmXn50qB8IzbR3BIAAA" onmousedown="addPPUrlParam &amp;&amp; addPPUrlParam(this);" rel="nofollow"><span class="pn"><span class="np"><svg width="24" height="24" fill="none"><path d="M10 6L8.59 7.41 13.17 12l-4.58 4.59L10 18l6-6-6-6z" fill="#2D2D2D"></path></svg></span></span></a></li></ul>
*/