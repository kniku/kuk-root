import { Component, OnInit, Directive, ViewRef, TemplateRef, ViewContainerRef, ViewChild, ComponentFactoryResolver, ComponentFactory, ComponentRef, ElementRef } from '@angular/core';

@Component({
  selector: 'app-test',
  templateUrl: './test.component.html',
  styleUrls: ['./test.component.css']
})
export class TestComponent implements OnInit {

  constructor() { }

  ngOnInit() {
  }

  public onTest()
  {
    var elem = document.getElementById("KURT");
    var b = new HTMLLabelElement();
    b.textContent = "KURT";
    elem.appendChild(b);
  }

}
