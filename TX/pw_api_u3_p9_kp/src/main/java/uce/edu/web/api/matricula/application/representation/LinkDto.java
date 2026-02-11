package uce.edu.web.api.matricula.application.representation;

public class LinkDto {
    public String rel;
    public String href;

    public LinkDto() {
    }

    public LinkDto(String rel, String href) {
        this.rel = rel;
        this.href = href;
    }
}
