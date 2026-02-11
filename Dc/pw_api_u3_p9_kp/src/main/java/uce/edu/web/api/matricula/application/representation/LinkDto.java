package uce.edu.web.api.matricula.application.representation;

import java.util.List;

public class LinkDto {
    private String href;
    private String rel;

    // constructor
    public LinkDto() {
    }

    public LinkDto(String href, String rel) {
        this.href = href;
        this.rel = rel;
    }

    public List<LinkDto> links;

    // getters y setters
    public String getHref() {
        return href;
    }

    public void setHref(String href) {
        this.href = href;
    }

    public String getRel() {
        return rel;
    }

    public void setRel(String rel) {
        this.rel = rel;
    }

    public List<LinkDto> getLinks() {
        return links;
    }

    public void setLinks(List<LinkDto> links) {
        this.links = links;
    }
}
